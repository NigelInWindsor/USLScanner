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
 *      SuppFunc.c
 *
 * Description:
 *
 *      Additional support functions
 *
 * Revision History:
 *
 *      06-01-08 : PLX SDK v6.00
 *
 ******************************************************************************/

#include "PlxChipFn.h"
#include "ApiFunc.h"
#include "GlobalVars.h"
#include "PciFunc.h"
#include "PlxChipApi.h"
#include "SuppFunc.h"
#include "DebugPrint.h"

#if defined(PLX_LOG_TO_FILE)
    #include <stdio.h>
    #include <stdarg.h>
#endif




#if defined(PLX_LOG_TO_FILE)
/*******************************************************************************
 *
 * Function   :  PlxLogPrintf
 *
 * Description:  Logs printf() style data to a file for debug purposes
 *
 ******************************************************************************/
VOID
PlxLogPrintf(
    const char *format,
    ...
    )
{
    int               i;
    int               j;
    int               StringLength;
    char              pOut[300];
    char              ConvFormat[300];
    HANDLE            hFile;
    va_list           pArgs;
    NTSTATUS          status;
    LARGE_INTEGER     ByteOffset;
    UNICODE_STRING    ObjectName;
    IO_STATUS_BLOCK   IoStatus;
    OBJECT_ATTRIBUTES ObjAttributes;


    // File functions are only supported at PASSIVE_LEVEL
    if (KeGetCurrentIrql() != PASSIVE_LEVEL)
        return;

    // Convert '\n' to '\r\n' to make MS-DOS text file
    i = 0;
    j = 0;

    while (format[i] != '\0')
    {
        if (format[i] == '\n')
        {
            ConvFormat[j] = '\r';
            j++;
        }

        // Copy character
        ConvFormat[j] = format[i];

        // Jump to next character
        i++;
        j++;
    }

    // Terminate string
    ConvFormat[j] = '\0';

    // Initialize the optional arguments pointer
    va_start(pArgs, format);

    // Build string to write
    StringLength = vsprintf(pOut, ConvFormat, pArgs);

    // Terminate arguments pointer
    va_end(pArgs);

    // Create the unicode file name
    RtlInitUnicodeString(
        &ObjectName,
        PLX_LOG_FILE_UNICODE
        );

    // Initialize file object
    InitializeObjectAttributes(
        &ObjAttributes,                // Attributes object
        &ObjectName,                   // Unicode object name
        OBJ_CASE_INSENSITIVE           // Attributes
         | OBJ_KERNEL_HANDLE,
        NULL,                          // Root directory handle
        NULL                           // Security Descriptor
        );

    // Open the file for appending output
    status =
        ZwCreateFile(
            &hFile,                    // File handle
            FILE_APPEND_DATA,          // Desired Access
            &ObjAttributes,            // Object attributes
            &IoStatus,                 // I/O status information
            NULL,                      // Allocation Size
            FILE_ATTRIBUTE_NORMAL,     // File Attributes
            0,                         // Share access
            FILE_OPEN_IF,              // Create disposition
            FILE_NON_DIRECTORY_FILE,   // Create options
            NULL,                      // EaBuffer
            0                          // EaLength
            );

    if (status != STATUS_SUCCESS)
    {
        DbgPrint((PLX_DRIVER_NAME ": ERROR: Unable to open or create log file\n"));
        return;
    }

    // Setup to write at end-of-file
    ByteOffset.HighPart = -1;
    ByteOffset.LowPart  = FILE_WRITE_TO_END_OF_FILE;

    // Write string to file
    status =
        ZwWriteFile(
            hFile,                     // File handle
            NULL,                      // Event handle
            NULL,                      // APC Routine
            NULL,                      // APC Context
            &IoStatus,                 // I/O status information
            pOut,                      // Buffer to write
            StringLength,              // Buffer length
            &ByteOffset,               // Byte offset
            NULL                       // Key
            );

    if (status != STATUS_SUCCESS)
    {
        DbgPrint((PLX_DRIVER_NAME ": ERROR: Unable to write to log file\n"));
    }

    // Close the file
    ZwClose( hFile );
}
#endif  // PLX_LOG_TO_FILE




/*******************************************************************************
 *
 * Function   :  Plx_sleep
 *
 * Description:  Function as a normal sleep. Parameter is in millisecond
 *
 ******************************************************************************/
VOID
Plx_sleep(
    U32 delay
    )
{
    LARGE_INTEGER liTime;


    /* Convert milliseconds to 100-nanosecond increments using:
     *
     *     1 ns  = 10 ^ -9 sec
     *   100 ns  = 10 ^ -7 sec (1 timer interval)
     *     1 ms  = 10 ^ -3 sec
     *     1 ms  = (1 timer interval) * 10^4
     */
    delay = delay * 10000;

    // Negative value means relative time, not absolute
    liTime.QuadPart = (S64)(-(LONG)delay);

    KeDelayExecutionThread(
        KernelMode,
        TRUE,
        &liTime
        );
}




/*******************************************************************************
 *
 * Function   :  PlxSynchronizedRegisterModify
 *
 * Description:  Synchronized function with ISR to modify a PLX register
 *
 ******************************************************************************/
BOOLEAN
PlxSynchronizedRegisterModify(
    PLX_REG_DATA *pRegData
    )
{
    U32 RegValue;


    RegValue =
        PLX_9000_REG_READ(
            pRegData->pdx,
            pRegData->offset
            );

    RegValue |= pRegData->BitsToSet;
    RegValue &= ~(pRegData->BitsToClear);

    PLX_9000_REG_WRITE(
        pRegData->pdx,
        pRegData->offset,
        RegValue
        );

    return TRUE;
}




/*******************************************************************************
 *
 * Function   :  PlxSignalNotifications
 *
 * Description:  Called by the DPC to signal any notification events
 *
 * Note       :  This is expected to be called at DPC level
 *
 ******************************************************************************/
VOID
PlxSignalNotifications(
    DEVICE_EXTENSION   *pdx,
    PLX_INTERRUPT_DATA *pIntData
    )
{
    U32              SourceDB;
    U32              SourceInt;
    PLIST_ENTRY      pEntry;
    PLX_WAIT_OBJECT *pWaitObject;


    KeAcquireSpinLockAtDpcLevel(
        &(pdx->Lock_WaitObjectsList)
        );

    // Get the interrupt wait list
    pEntry = pdx->List_WaitObjects.Flink;

    // Traverse wait objects and wake-up processes
    while (pEntry != &(pdx->List_WaitObjects))
    {
        // Get the wait object
        pWaitObject =
            CONTAINING_RECORD(
                pEntry,
                PLX_WAIT_OBJECT,
                ListEntry
                );

        // Set active notifications
        SourceInt = pWaitObject->Notify_Flags & pIntData->Source_Ints;
        SourceDB  = pWaitObject->Notify_Doorbell & pIntData->Source_Doorbell;

        // Check if waiting for active interrupt
        if (SourceInt || SourceDB)
        {
            DebugPrintf((
                "DPC signaling wait object (%p)\n",
                pWaitObject
                ));

            // Save new interrupt sources in case later requested
            pWaitObject->Source_Ints     |= SourceInt;
            pWaitObject->Source_Doorbell |= SourceDB;

            // Signal wait object
            KeSetEvent(
                pWaitObject->pKEvent,
                IO_NO_INCREMENT,
                FALSE
                );
        }

        // Jump to next item in the list
        pEntry = pEntry->Flink;
    }

    KeReleaseSpinLockFromDpcLevel(
        &(pdx->Lock_WaitObjectsList)
        );
}




/*******************************************************************************
 *
 * Function   :  PlxRegistryInformationGet
 *
 * Description:  Gets driver configuration information from the registry
 *
 ******************************************************************************/
VOID
PlxRegistryInformationGet(
    UNICODE_STRING    *pRegistryPath,
    PLX_REGISTRY_INFO *pRegistryInfo
    )
{
    NTSTATUS                  status;
    RTL_QUERY_REGISTRY_TABLE  RegTable[2];


    RtlZeroMemory(
        RegTable,
        sizeof(RegTable)
        );

    RegTable[0].Flags = RTL_QUERY_REGISTRY_REQUIRED | RTL_QUERY_REGISTRY_DIRECT;

    // Get Common buffer size
    RegTable[0].Name         = L"CommonBufferSize";
    RegTable[0].EntryContext = &(pRegistryInfo->Size_CommonBuffer);

    status =
        RtlQueryRegistryValues(
            RTL_REGISTRY_ABSOLUTE,
            pRegistryPath->Buffer,
            RegTable,
            NULL,
            NULL
            );

    if ( !NT_SUCCESS(status) )
    {
        pRegistryInfo->Size_CommonBuffer = DEFAULT_SIZE_COMMON_BUFFER;
    }
}




/*******************************************************************************
 *
 * Function   :  GetBarIndex
 *
 * Description:  Determine which PCI BAR contains the specified address
 *
 ******************************************************************************/
U8
GetBarIndex(
    DEVICE_EXTENSION *pdx,
    U32               BarValue,
    U32              *pPciBars
    )
{
    U8  i;
    U32 CompareAddress;


    // Compare the physical address with each BAR
    for (i = 0; i < PCI_NUM_BARS_TYPE_00; i++)
    {
        if (pPciBars[i] & (1 << 0))
            CompareAddress = pPciBars[i] & ~(0x3);
        else
            CompareAddress = pPciBars[i] & ~(0xF);

        if (BarValue == CompareAddress)
            return i;
    }

    // Unable to find the BAR index
    DebugPrintf((
        "ERROR - GetBarIndex() unable to match BAR value (0x%08lX)\n",
        BarValue
        ));

    return (U8)-1;
}




/*******************************************************************************
 *
 * Function   :  PlxCompleteIrp
 *
 * Description:  Complete an IRP
 *
 ******************************************************************************/
NTSTATUS
PlxCompleteIrp(
    PIRP     pIrp,
    NTSTATUS status
    )
{
	PIO_STACK_LOCATION  pStack;
	pStack =IoGetCurrentIrpStackLocation(pIrp);

//	DebugPrint("PlxCompleteIrp IOCTL %x",pStack->Parameters.DeviceIoControl.IoControlCode);

    pIrp->IoStatus.Status = status;

    IoCompleteRequest(
        pIrp,
        IO_NO_INCREMENT
        );

    if (status == STATUS_CANCELLED)
        DebugPrintf(("...Cancelled IRP (0x%p)\n", pIrp));
    else
        DebugPrintf(("...Completed IRP (0x%p)\n", pIrp));

    return status;
}




/*******************************************************************************
 *
 * Function   :  PlxCompleteIrpWithInformation
 *
 * Description:  Complete an IRP including modification of the Information field
 *
 ******************************************************************************/
NTSTATUS
PlxCompleteIrpWithInformation(
    PIRP         pIrp,
    NTSTATUS     status,
    PLX_UINT_PTR Info
    )
{
	PIO_STACK_LOCATION  pStack;
	pStack =IoGetCurrentIrpStackLocation(pIrp);

//	DebugPrint("PlxCompleteIrpWithInformation IOCTL %x",pStack->Parameters.DeviceIoControl.IoControlCode);

    pIrp->IoStatus.Status      = status;
    pIrp->IoStatus.Information = Info;

    IoCompleteRequest(
        pIrp,
        IO_NO_INCREMENT
        );

    if (status == STATUS_CANCELLED)
        DebugPrintf(("...Cancelled IRP (0x%p)\n", pIrp));
    else
        DebugPrintf(("...Completed IRP (0x%p)\n", pIrp));

    return status;
}




/*******************************************************************************
 *
 * Function   :  PlxGetExtendedCapabilityOffset
 *
 * Description:  Attempts to detect the PLX chip revision
 *
 ******************************************************************************/
U16
PlxGetExtendedCapabilityOffset(
    DEVICE_EXTENSION *pdx,
    U16               CapabilityId
    )
{
    U16 Offset_Cap;
    U32 RegValue;


    // Get offset of first capability
    PLX_PCI_REG_READ(
        pdx,
        0x34,           // PCI capabilities pointer
        &RegValue
        );

    // If link is down, PCI reg accesses will fail
    if (RegValue == (U32)-1)
        return 0;

    // Set first capability
    Offset_Cap = (U16)RegValue;

    // Traverse capability list searching for desired ID
    while ((Offset_Cap != 0) && (RegValue != (U32)-1))
    {
        // Get next capability
        PLX_PCI_REG_READ(
            pdx,
            Offset_Cap,
            &RegValue
            );

        if ((U8)RegValue == (U8)CapabilityId)
        {
            // Capability found, return base offset
            return Offset_Cap;
        }

        // Jump to next capability
        Offset_Cap = (U16)((RegValue >> 8) & 0xFF);
    }

    // Capability not found
    return 0;
}




/*******************************************************************************
 *
 * Function   :  PlxUpdateDeviceKey
 *
 * Description:  Updates device key information
 *
 ******************************************************************************/
VOID
PlxUpdateDeviceKey(
    DEVICE_EXTENSION *pdx
    )
{
    U32 RegValue;


    // Read the Device/Vendor ID
    PLX_PCI_REG_READ(
        pdx,
        0,
        &RegValue
        );

    // Store device/vendor IDs
    pdx->Key.VendorId = (U16)RegValue;
    pdx->Key.DeviceId = (U16)(RegValue >> 16);

    // Update Subsystem Device/Vendor ID
    PLX_PCI_REG_READ(
        pdx,
        0x2c,        // PCI Subsystem ID
        &RegValue
        );

    pdx->Key.SubVendorId = (U16)(RegValue & 0xffff);
    pdx->Key.SubDeviceId = (U16)(RegValue >> 16);

    // Update Revision ID
    PLX_PCI_REG_READ(
        pdx,
        0x08,        // PCI Revision ID
        &RegValue
        );

    pdx->Key.Revision = (U8)(RegValue & 0xFF);
}




/*******************************************************************************
 *
 * Function   :  PlxPowerStateIsSupported
 *
 * Description:  Verify whether the device supports a power state
 *
 ******************************************************************************/
BOOLEAN
PlxPowerStateIsSupported(
    DEVICE_EXTENSION   *pdx,
    DEVICE_POWER_STATE  PowerState
    )
{
    U16 Offset_PM;
    U32 RegValue;


    // Verify that the Power State is supported
    switch (PowerState)
    {
        case PowerDeviceD0:
        case PowerDeviceD3:
            // These states supported by all devices
            return TRUE;

        case PowerDeviceD1:
        case PowerDeviceD2:
            // Check if device supports Power Management capability
            Offset_PM =
                PlxGetExtendedCapabilityOffset(
                    pdx,
                    CAP_ID_POWER_MAN
                    );

            if (Offset_PM == 0)
            {
                return FALSE;
            }

            // Get the PM Capabilities
            PLX_PCI_REG_READ(
                pdx,
                Offset_PM,
                &RegValue
                );

            // Check supported power states as reported by device
            if  (((PowerState == PowerDeviceD1) && (RegValue & (1 << 25))) ||
                 ((PowerState == PowerDeviceD2) && (RegValue & (1 << 26))))
            {
                return TRUE;
            }
            break;

        case PowerDeviceUnspecified:
        default:
            break;
    }

    return FALSE;
}




/*******************************************************************************
 *
 * Function   :  PlxPowerStateSet
 *
 * Description:  Set the device power state
 *
 ******************************************************************************/
PLX_STATUS
PlxPowerStateSet(
    DEVICE_EXTENSION   *pdx,
    DEVICE_POWER_STATE  PowerState
    )
{
    U16 Offset_PM;
    U32 RegValue;


    // Check if New capabilities are enabled
    Offset_PM =
        PlxGetExtendedCapabilityOffset(
            pdx,
            CAP_ID_POWER_MAN
            );

    if (Offset_PM == 0)
    {
        return ApiUnsupportedFunction;
    }

    // Check if we support the power state
    if (PlxPowerStateIsSupported(
            pdx,
            PowerState
            ) == FALSE)
    {
        return ApiInvalidPowerState;
    }

    // Adjust offset to PM Control/Status register
    Offset_PM += sizeof(U32);

    DebugPrintf(("Putting device into state ==> "));

    // Get the power state
    PLX_PCI_REG_READ(
        pdx,
        Offset_PM,
        &RegValue
        );

    // Clear power state (bits [1:0])
    RegValue = (RegValue & ~0x3);

    // Set new power state (bits [1:0])
    if (PowerState == PowerDeviceD0)
    {
        DebugPrintf_NoInfo(("D0\n"));
        RegValue |= 0;
    }
    else if (PowerState == PowerDeviceD1)
    {
        DebugPrintf_NoInfo(("D1\n"));
        RegValue |= 1;
    }
    else if (PowerState == PowerDeviceD2)
    {
        DebugPrintf_NoInfo(("D2\n"));
        RegValue |= 2;
    }
    else if (PowerState == PowerDeviceD3)
    {
        DebugPrintf_NoInfo(("D3\n"));
        RegValue |= 3;
    }

    // Set the new power state
    PLX_PCI_REG_WRITE(
        pdx,
        Offset_PM,
        RegValue
        );

    return ApiSuccess;
}




/*******************************************************************************
 *
 * Function   :  PlxPciBarResourceMap
 *
 * Description:  Maps a PCI BAR resource into kernel space
 *
 ******************************************************************************/
NTSTATUS
PlxPciBarResourceMap(
    DEVICE_EXTENSION *pdx,
    U8                BarIndex
    )
{
    U32                  SizeToMap;
    U32                  SizeRemain;
    VOID                *pVa;
    PMDL                 pMdl;
    PMDL                 pMdl_Previous;
    PHYSICAL_ADDRESS     AddrPhysical;
    MEMORY_CACHING_TYPE  CacheMode;


    /***********************************************************
     * For pre-fetchable region, enable write-combining
     * Cached mode seems to have no effect, but write-combining
     * results in much better CPU write performance.
     **********************************************************/
    if (pdx->PciBar[BarIndex].Properties.bPrefetchable)
        CacheMode = MmWriteCombined;
    else
        CacheMode = MmNonCached;

    // Setup physical address for mapping
    AddrPhysical.QuadPart = pdx->PciBar[BarIndex].Properties.Physical;

    // Map into Kernel Virtual Space
    pdx->PciBar[BarIndex].pVa =
        MmMapIoSpace(
            AddrPhysical,
            (ULONG)pdx->PciBar[BarIndex].Properties.Size,
            CacheMode
            );

    if (pdx->PciBar[BarIndex].pVa == NULL)
    {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    /******************************************************************
     * A future mapping into user space will require an MDL. Due to
     * the limited region a single MDL can describe, multiple MDL's
     * must be allocated and chained together via the 'Next' field.
     *****************************************************************/

    // Set initial values
    pVa        = pdx->PciBar[BarIndex].pVa;
    SizeRemain = (U32)pdx->PciBar[BarIndex].Properties.Size;

    while (SizeRemain != 0)
    {
        // Determine size for MDL
        if (SizeRemain <= MAX_MDL_SIZE)
        {
            SizeToMap = SizeRemain;
        }
        else
        {
            SizeToMap = MAX_MDL_SIZE;
        }

        // Get an MDL for future mapping into user space
        pMdl =
            IoAllocateMdl(
                pVa,
                SizeToMap,
                FALSE,          // Is this a secondary buffer?
                FALSE,          // Charge quota?
                NULL            // No IRP associated with MDL
                );

        // Check if the MDL allocation succeeded
        if (pMdl == NULL)
        {
            DebugPrintf(("ERROR - MDL allocation for space failed\n"));

            // Release any created MDLs
            while (pdx->PciBar[BarIndex].pMdl != NULL)
            {
                pMdl = pdx->PciBar[BarIndex].pMdl;

                pdx->PciBar[BarIndex].pMdl = pMdl->Next;

                IoFreeMdl( pMdl );
            }

            return STATUS_SUCCESS;
        }

        // Build the MDL
        MmBuildMdlForNonPagedPool( pMdl );

        // Clear next MDL pointer
        pMdl->Next = NULL;

        // Store MDL
        if (pdx->PciBar[BarIndex].pMdl == NULL)
        {
            // Insert first MDL
            pdx->PciBar[BarIndex].pMdl = pMdl;
        }
        else
        {
            // Add new MDL to list
            pMdl_Previous->Next = pMdl;
        }

        // Update previous pointer
        pMdl_Previous = pMdl;

        // Adjust for next virtual address
        (U8*)pVa += SizeToMap;

        // Adjust remaining count
        SizeRemain -= SizeToMap;
    }

    return STATUS_SUCCESS;
}




/*******************************************************************************
 *
 * Function   :  PlxPciBarResourcesUnmap
 *
 * Description:  Unmap all mapped PCI BAR memory for a device
 *
 ******************************************************************************/
VOID
PlxPciBarResourcesUnmap(
    DEVICE_EXTENSION *pdx
    )
{
    U8   i;
    PMDL pMdl;


    // Go through all the BARS
    for (i = 0; i < PCI_NUM_BARS_TYPE_00; i++)
    {
        // Unmap the space from Kernel space if previously mapped
        if ((pdx->PciBar[i].Properties.bIoSpace == FALSE) &&
            (pdx->PciBar[i].Properties.Physical != 0))
        {
            DebugPrintf((
                "Unmapping BAR %d (VA=%p)...\n",
                i, pdx->PciBar[i].pVa
                ));

            // Release the MDLs describing the BAR space
            while (pdx->PciBar[i].pMdl != NULL)
            {
                pMdl = pdx->PciBar[i].pMdl;

                pdx->PciBar[i].pMdl = pMdl->Next;

                IoFreeMdl( pMdl );
            }

            // Unmap from kernel space
            if (pdx->PciBar[i].pVa != NULL)
            {
                MmUnmapIoSpace(
                    pdx->PciBar[i].pVa,
                    (ULONG)pdx->PciBar[i].Properties.Size
                    );
                pdx->PciBar[i].pVa = NULL;
            }
        }
    }
}




/*******************************************************************************
 *
 * Function   :  PlxPciBarSpaceUnmapAll_ByOwner
 *
 * Description:  Unmap any PCI BAR spaces assigned to the specified owner
 *
 ******************************************************************************/
VOID
PlxPciBarSpaceUnmapAll_ByOwner(
    DEVICE_EXTENSION *pdx,
    VOID             *pOwner
    )
{
    VOID             *UserVa;
    KIRQL             IrqL_Original;
    PLIST_ENTRY       pEntry;
    PLX_USER_MAPPING *pMapObject;


    KeAcquireSpinLock(
        &(pdx->Lock_BarMappingsList),
        &IrqL_Original
        );

    pEntry = pdx->List_BarMappings.Flink;

    // Traverse list to find the desired list objects
    while (pEntry != &(pdx->List_BarMappings))
    {
        // Get the object
        pMapObject =
            CONTAINING_RECORD(
                pEntry,
                PLX_USER_MAPPING,
                ListEntry
                );

        // Check if owner matches
        if (pMapObject->pOwner == pOwner)
        {
            // Copy address
            UserVa = pMapObject->pUserVa[0];

            // Release list lock
            KeReleaseSpinLock(
                &(pdx->Lock_BarMappingsList),
                IrqL_Original
                );

            // Unmap BAR space
            PlxPciBarUnmap(
                pdx,
                &pdx->Key,
                UserVa,
                pOwner
                ); 

            KeAcquireSpinLock(
                &(pdx->Lock_BarMappingsList),
                &IrqL_Original
                );

            // Restart parsing the list from the beginning
            pEntry = pdx->List_BarMappings.Flink;
        }
        else
        {
            // Jump to next item
            pEntry = pEntry->Flink;
        }
    }

    KeReleaseSpinLock(
        &(pdx->Lock_BarMappingsList),
        IrqL_Original
        );
}




/*******************************************************************************
 *
 * Function   :  PlxPciPhysicalMemoryFreeAll_ByOwner
 *
 * Description:  Unmap & release all physical memory assigned to the specified owner
 *
 ******************************************************************************/
VOID
PlxPciPhysicalMemoryFreeAll_ByOwner(
    DEVICE_EXTENSION *pdx,
    VOID             *pOwner
    )
{
    KIRQL                IrqL_Original;
    PLIST_ENTRY          pEntry;
    PLX_PHYSICAL_MEM     PciMem;
    PLX_PHYS_MEM_OBJECT *pMemObject;


    KeAcquireSpinLock(
        &(pdx->Lock_PhysicalMemList),
        &IrqL_Original
        );

    pEntry = pdx->List_PhysicalMem.Flink;

    // Traverse list to find the desired list objects
    while (pEntry != &(pdx->List_PhysicalMem))
    {
        // Get the object
        pMemObject =
            CONTAINING_RECORD(
                pEntry,
                PLX_PHYS_MEM_OBJECT,
                ListEntry
                );

        // Check if owner matches
        if (pMemObject->pOwner == pOwner)
        {
            // Copy memory information
            PciMem.PhysicalAddr = pMemObject->BusPhysical;
            PciMem.Size         = pMemObject->Size;

            // Release list lock
            KeReleaseSpinLock(
                &(pdx->Lock_PhysicalMemList),
                IrqL_Original
                );

            // Release the memory & remove from list
            PlxPciPhysicalMemoryFree(
                pdx,
                &PciMem
                ); 

            KeAcquireSpinLock(
                &(pdx->Lock_PhysicalMemList),
                &IrqL_Original
                );

            // Restart parsing the list from the beginning
            pEntry = pdx->List_PhysicalMem.Flink;
        }
        else
        {
            // Jump to next item
            pEntry = pEntry->Flink;
        }
    }

    KeReleaseSpinLock(
        &(pdx->Lock_PhysicalMemList),
        IrqL_Original
        );
}




/*******************************************************************************
 *
 * Function   :  PlxPciPhysicalMemoryUnmapAll_ByOwner
 *
 * Description:  Unmap any physical memory assigned to the specified owner
 *
 ******************************************************************************/
VOID
PlxPciPhysicalMemoryUnmapAll_ByOwner(
    DEVICE_EXTENSION    *pdx,
    PLX_PHYS_MEM_OBJECT *pMemObject,
    VOID                *pOwner
    )
{
    VOID             *pUserVa;
    KIRQL             IrqL_Original;
    PLIST_ENTRY       pEntry;
    PLX_PHYSICAL_MEM  PciMem;
    PHYSICAL_ADDRESS  AddrPhysical;
    PLX_USER_MAPPING *pMapObject;


    // Verify Memory Object
    if (pMemObject == NULL)
    {
        return;
    }

    // Verify size
    if (pMemObject->Size == 0)
    {
        return;
    }

    // Setup memory properties
    PciMem.PhysicalAddr = pMemObject->BusPhysical;
    PciMem.Size         = pMemObject->Size;

    // Find the map object to unmap
    KeAcquireSpinLock(
        &(pMemObject->Lock_MappingsList),
        &IrqL_Original
        );

    pEntry = pMemObject->List_Mappings.Flink;

    // Traverse list to find the desired list object
    while (pEntry != &(pMemObject->List_Mappings))
    {
        // Get the object
        pMapObject =
            CONTAINING_RECORD(
                pEntry,
                PLX_USER_MAPPING,
                ListEntry
                );

        // Check if the owner matches
        if ((pOwner == NULL) || (pMapObject->pOwner == pOwner))
        {
            // Copy virtual address
            PciMem.UserAddr = (PLX_UINT_PTR)pMapObject->pUserVa[0];

            // Release list lock
            KeReleaseSpinLock(
                &(pMemObject->Lock_MappingsList),
                IrqL_Original
                );

            // Unmap the memory & remove from list
            PlxPciPhysicalMemoryUnmap(
                pdx,
                &PciMem,
                pMapObject->pOwner
                ); 

            KeAcquireSpinLock(
                &(pMemObject->Lock_MappingsList),
                &IrqL_Original
                );

            // Restart parsing the list from the beginning
            pEntry = pMemObject->List_Mappings.Flink;
        }
        else
        {
            // Jump to next item
            pEntry = pEntry->Flink;
        }
    }

    KeReleaseSpinLock(
        &(pMemObject->Lock_MappingsList),
        IrqL_Original
        );
}




/*******************************************************************************
 *
 * Function   :  Plx_dma_buffer_alloc
 *
 * Description:  Allocates physically contiguous non-paged memory
 *
 ******************************************************************************/
VOID*
Plx_dma_buffer_alloc(
    DEVICE_EXTENSION    *pdx,
    PLX_PHYS_MEM_OBJECT *pMemObject
    )
{
    PHYSICAL_ADDRESS BufferLogicalAddress;


    // Verify the DMA adapter object
    if (pdx->pDmaAdapter == NULL)
    {
        DebugPrintf((
            "ERROR - DMA Adapter object does not exist, cannot allocate physical memory\n"
            ));

        return NULL;
    }

    // Attempt to allocate contiguous memory
    pMemObject->pKernelVa =
        pdx->pDmaAdapter->DmaOperations->AllocateCommonBuffer(
            pdx->pDmaAdapter,
            pMemObject->Size,
            &BufferLogicalAddress,
            pMemObject->bCacheable      // Enable Caching for buffer?
            );

    if (pMemObject->pKernelVa == NULL)
    {
        return NULL;
    }

    // Store the buffer logical/bus physical address
    pMemObject->BusPhysical = BufferLogicalAddress.QuadPart;

    // CPU Address is the same as bus on this architecture
    pMemObject->CpuPhysical = BufferLogicalAddress.QuadPart;

    // Clear the buffer
    RtlZeroMemory(
        pMemObject->pKernelVa,
        pMemObject->Size
        );

    DebugPrintf(("Allocated physical memory...\n"));

    DebugPrintf((
        "    CPU Phys Addr: 0x%08lx\n",
        (PLX_UINT_PTR)pMemObject->CpuPhysical
        ));

    DebugPrintf((
        "    Bus Phys Addr: 0x%08lx\n",
        (PLX_UINT_PTR)pMemObject->BusPhysical
        ));

    DebugPrintf((
        "    Kernel VA    : 0x%p\n",
        pMemObject->pKernelVa
        ));

    DebugPrintf((
        "    Size         : "
        ));

    if (pMemObject->Size >= (10 << 10))
    {
        DebugPrintf_NoInfo((
            "%d Kb\n",
            (pMemObject->Size >> 10)
            ));
    }
    else
    {
        DebugPrintf_NoInfo((
            "%d bytes\n",
            pMemObject->Size
            ));
    }

    DebugPrintf((
        "    Cacheable?   : %s\n",
        (pMemObject->bCacheable == FALSE) ? "No" : "Yes"
        ));

    // Return the kernel virtual address of the allocated block
    return pMemObject->pKernelVa;
}




/*******************************************************************************
 *
 * Function   :  Plx_dma_buffer_free
 *
 * Description:  Frees previously allocated physical memory
 *
 ******************************************************************************/
VOID
Plx_dma_buffer_free(
    DEVICE_EXTENSION    *pdx,
    PLX_PHYS_MEM_OBJECT *pMemObject
    )
{
    PHYSICAL_ADDRESS  BufferLogicalAddress;


    // Verify the DMA adapter object
    if (pdx->pDmaAdapter == NULL)
    {
        DebugPrintf((
            "ERROR - DMA Adapter object does not exist, cannot allocate physical memory\n"
            ));

        return;
    }

    // Set logical address
    BufferLogicalAddress.QuadPart = pMemObject->BusPhysical;

    // Release the buffer
    pdx->pDmaAdapter->DmaOperations->FreeCommonBuffer(
        pdx->pDmaAdapter,
        pMemObject->Size,
        BufferLogicalAddress,
        pMemObject->pKernelVa,
        pMemObject->bCacheable      // Enable Caching for buffer?
        );

    DebugPrintf((
        "Released physical memory at 0x%08lx ",
        (PLX_UINT_PTR)pMemObject->CpuPhysical
        ));

    if (pMemObject->Size >= (10 << 10))
    {
        DebugPrintf_NoInfo((
            "(%d Kb)\n",
            (pMemObject->Size >> 10)
            ));
    }
    else
    {
        DebugPrintf_NoInfo((
            "(%d bytes)\n",
            pMemObject->Size
            ));
    }

    // Clear memory object properties
    RtlZeroMemory(
        pMemObject,
        sizeof(PLX_PHYS_MEM_OBJECT)
        );
}




/*******************************************************************************
 *
 * Function   :  PlxDmaChannelCleanup
 *
 * Description:  Called by the Cleanup routine to close any open channels
 *
 ******************************************************************************/
VOID
PlxDmaChannelCleanup(
    DEVICE_EXTENSION *pdx,
    VOID             *pOwner
    )
{
#if defined(PLX_DMA_SUPPORT)
    U8 i;


    for (i = 0; i < NUMBER_OF_DMA_CHANNELS; i++)
    {
        // Check if terminating application is the owner
        if (pdx->DmaInfo[i].pOwner == pOwner)
        {
            if (pdx->DmaInfo[i].bOpen)
            {
                PlxChip_DmaChannelClose(
                    pdx,
                    i,
                    FALSE
                    );
            }
        }
    }
#endif  // PLX_DMA_SUPPORT
}




#if defined(PLX_DMA_SUPPORT)
/*******************************************************************************
 *
 * Function   :  PlxSglDmaTransferComplete
 *
 * Description:  Perform any necessary cleanup after an SGL DMA transfer
 *
 ******************************************************************************/
VOID
PlxSglDmaTransferComplete(
    DEVICE_EXTENSION *pdx,
    U8                channel
    )
{
    KIRQL IrqL_Original;


    if (pdx->DmaInfo[channel].bSglPending == FALSE)
    {
        DebugPrintf(("No pending SGL DMA to complete\n"));
        return;
    }

    DebugPrintf(("Unlocking user-mode buffer used for SGL DMA transfer...\n"));

    KeAcquireSpinLock(
        &(pdx->Lock_DmaChannel),
        &IrqL_Original
        );

    // Unlock the data buffer and free the MDL
    if (pdx->DmaInfo[channel].pMdl != NULL)
    {
        MmUnlockPages(
            pdx->DmaInfo[channel].pMdl
            );

        IoFreeMdl(
            pdx->DmaInfo[channel].pMdl
            );

        pdx->DmaInfo[channel].pMdl = NULL;
    }

    // Clear the DMA pending flag
    pdx->DmaInfo[channel].bSglPending = FALSE;

    KeReleaseSpinLock(
        &(pdx->Lock_DmaChannel),
        IrqL_Original
        );
}




/*******************************************************************************
 *
 * Function   :  PlxLockBufferAndBuildSgl
 *
 * Description:  Lock a user buffer and build an SGL for it
 *
 ******************************************************************************/
PLX_STATUS
PlxLockBufferAndBuildSgl(
    DEVICE_EXTENSION *pdx,
    U8                channel,
    PLX_DMA_PARAMS   *pDma,
    U32              *pSglAddress,
    BOOLEAN          *pbBits64
    )
{
    U8            SizeDescr;
    U32           i;
    U32           offset;
    U32           BusSgl;
    U32           BusSglOriginal;
    U32           SglSize;
    U32           BlockSize;
    U32           LocalAddr;
    U32           TotalPages;
    U32           BytesRemaining;
    U64           BusAddr;
    U64           MaskBits64;
    PMDL          pMdl;
    KIRQL         IrqL_Original;
    BOOLEAN       bFlag;
    PLX_UINT_PTR  VaSgl;
    PLX_UINT_PTR *pCurrentPage;


    DebugPrintf(("Building SGL descriptors for buffer...\n"));
    DebugPrintf(("   User VA   : 0x%08lx\n", (PLX_UINT_PTR)pDma->u.UserVa));
    DebugPrintf(("   Local Addr: 0x%08x\n", pDma->LocalAddr));
    DebugPrintf(("   Size      : %d bytes\n", pDma->ByteCount));

    // Set default return address
    *pSglAddress = 0;

    // Get the MDL for the User buffer to get the page list
    pMdl =
        IoAllocateMdl(
            (VOID*)pDma->u.UserVa,
            pDma->ByteCount,
            FALSE,
            FALSE,
            NULL
            );

    if (pMdl == NULL)
    {
        DebugPrintf(("ERROR - Unable to allocate an MDL for the User buffer\n"));
        return ApiDmaSglPagesGetError;
    }

    // Attempt to lock the user buffer into memory
    bFlag = FALSE;

    try
    {
        MmProbeAndLockPages(
            pMdl,
            UserMode,
            IoModifyAccess
            );
    }
    except(EXCEPTION_EXECUTE_HANDLER)
    {
        // Flag the exception
        bFlag = TRUE;
    }

    // Verify that the pages were locked
    if (bFlag)
    {
        DebugPrintf(("ERROR - Unable to lock user mode buffer pages\n"));
        IoFreeMdl( pMdl );
        return ApiDmaSglPagesLockError;
    }

    // Calculate total pages
    TotalPages = (((U16)pMdl->Size - sizeof(MDL)) / sizeof(PLX_UINT_PTR));

    DebugPrintf((
        "Page-locked %d user buffer pages...\n",
        TotalPages
        ));

    /*************************************************************
     * Parse the entire page list to determine whether dual address
     * descriptors are needed to support 64-bit transfers.
     ************************************************************/
    // Default to 32-bit transfer
    *pbBits64 = FALSE;

    // Calculate a mask to check for non-zero upper 32-bit address
    MaskBits64 = ~((((U64)1 << 32) >> PAGE_SHIFT) - 1);

    // Point to first page in physical page list (located at end of MDL)
    pCurrentPage = (PLX_UINT_PTR*)(((U8*)pMdl) + sizeof(MDL));

    // Check each page address for non-zero upper 32-bit address
    for (i = 0; i < TotalPages; i++)
    {
        // If upper bits exist, default to 64-bit DMA
        if (*pCurrentPage & MaskBits64)
        {
            *pbBits64 = TRUE;

            // Verify device supports dual-addressing
            if (pdx->PlxChipType == 0x9080)
            {
                DebugPrintf((
                    "ERROR: PLX %04X device does not support 64-bit DMA transfers\n",
                    pdx->PlxChipType
                    ));

                IoFreeMdl( pMdl );
                MmUnlockPages( pMdl );
                return ApiUnsupportedFunction;
            }

            DebugPrintf(("One or more pages located in 64-bit space, enabling dual-addressing\n"));
            break;
        }

        // Go to next page
        pCurrentPage++;
    }

    /*************************************************************
     * Build SGL descriptors
     *
     * The following code will build the SGL descriptors
     * in PCI memory.  There will be one descriptor for
     * each page of memory since the pages are scattered
     * throughout physical memory.
     ************************************************************/

    /*************************************************************
     * Calculate memory needed for SGL descriptors
     *
     * Mem needed = (#pages * size of descriptor) + (rounding bytes)
     *
     * Additionally, a value of 16 or 32 is added to provide a
     * buffer to allow space to round up to the next 16-byte or
     * 32-byte boundary, which is a requirement of the hardware.
     ************************************************************/

    // Calculate descriptor size
    if (*pbBits64)
        SizeDescr = 8 * sizeof(U32);
    else
        SizeDescr = 4 * sizeof(U32);

    // Calculate SGL size
    SglSize = (TotalPages * SizeDescr) + SizeDescr;

    // Check if a previously allocated buffer can be re-used
    if (pdx->DmaInfo[channel].SglBuffer.pKernelVa != NULL)
    {
        if (pdx->DmaInfo[channel].SglBuffer.Size >= SglSize)
        {
            // Buffer can be re-used, do nothing
            DebugPrintf(("Re-using previously allocated SGL descriptor buffer\n"));
        }
        else
        {
            DebugPrintf(("Releasing previously allocated SGL descriptor buffer\n"));

            // Release memory used for SGL descriptors
            Plx_dma_buffer_free(
                pdx,
                &pdx->DmaInfo[channel].SglBuffer
                );

            pdx->DmaInfo[channel].SglBuffer.pKernelVa = NULL;
        }
    }

    // Allocate memory for SGL descriptors if necessary
    if (pdx->DmaInfo[channel].SglBuffer.pKernelVa == NULL)
    {
        DebugPrintf(("Allocating PCI memory for SGL descriptor buffer...\n"));

        // Setup for transfer
        pdx->DmaInfo[channel].SglBuffer.Size       = SglSize;
        pdx->DmaInfo[channel].SglBuffer.bCacheable = FALSE;

        VaSgl =
            (PLX_UINT_PTR)Plx_dma_buffer_alloc(
                pdx,
                &pdx->DmaInfo[channel].SglBuffer
                );

        if (VaSgl == 0)
        {
            DebugPrintf((
                "ERROR - Unable to allocate %d bytes for %d SGL descriptors\n",
                pdx->DmaInfo[channel].SglBuffer.Size,
                TotalPages
                ));

            MmUnlockPages( pMdl );
            IoFreeMdl( pMdl );
            return ApiInsufficientResources;
        }
    }
    else
    {
        VaSgl = (PLX_UINT_PTR)pdx->DmaInfo[channel].SglBuffer.pKernelVa;
    }


    // Save MDL for deallocation after the DMA transfer
    KeAcquireSpinLock(
        &(pdx->Lock_DmaChannel),
        &IrqL_Original
        );

    pdx->DmaInfo[channel].pMdl = pMdl;

    KeReleaseSpinLock(
        &(pdx->Lock_DmaChannel),
        IrqL_Original
        );

    // Prepare for build of SGL
    LocalAddr = pDma->LocalAddr;

    // Get bus physical address of SGL descriptors
    BusSgl = (U32)pdx->DmaInfo[channel].SglBuffer.BusPhysical;

    // Make sure addresses are aligned on next descriptor boundary
    VaSgl  = (VaSgl + (SizeDescr - 1)) & ~((SizeDescr - 1));
    BusSgl = (BusSgl + (SizeDescr - 1)) & ~((SizeDescr - 1));

    // Store the starting address of the SGL for later return
    BusSglOriginal = BusSgl;

    DebugPrintf((
        "Building SGL descriptor list located at 0x%08x...\n",
        BusSglOriginal
        ));

    // Point to first page in physical page list (located at end of MDL)
    pCurrentPage = (PLX_UINT_PTR*)(((U8*)pMdl) + sizeof(MDL));

    // Set offset of first page
    offset = pMdl->ByteOffset;

    // Initialize bytes remaining
    BytesRemaining = pDma->ByteCount;

    // Build the SGL list
    for (i = 0; i < TotalPages; i++)
    {
        // Calculate transfer size
        if (BytesRemaining > (PAGE_SIZE - offset))
        {
            BlockSize = PAGE_SIZE - offset;
        }
        else
        {
            BlockSize = BytesRemaining;
        }

        // Get bus address of buffer
        BusAddr = (((U64)*pCurrentPage << PAGE_SHIFT) + offset);

        // Enable the following to display the parameters of each SGL descriptor
        if (PLX_DEBUG_DISPLAY_SGL_DESCR)
        {
            DebugPrintf((
                "SGL Desc %02d: PCI=%08I64X  Loc=%08X  Size=%X (%d) bytes\n",
                i, (U64)BusAddr, LocalAddr, BlockSize, BlockSize
                ));
        }

        // Write PCI address in descriptor
        *(((U32*)VaSgl) + SGL_DESC_IDX_PCI_LOW) = PLX_LE_DATA_32( (U32)BusAddr );

        // Write upper 32-bit of 64-bit PCI address in descriptor
        if (*pbBits64)
            *(((U32*)VaSgl) + SGL_DESC_IDX_PCI_HIGH) = PLX_LE_DATA_32( (U32)(BusAddr >> 32) );

        // Write Local address in descriptor
        *(((U32*)VaSgl) + SGL_DESC_IDX_LOC_ADDR) = PLX_LE_DATA_32( LocalAddr );

        // Write transfer count in descriptor
        *(((U32*)VaSgl) + SGL_DESC_IDX_COUNT) = PLX_LE_DATA_32( BlockSize );

        // Adjust byte count
        BytesRemaining -= BlockSize;

        if (BytesRemaining == 0)
        {
            // Write the last descriptor
            *(((U32*)VaSgl) + SGL_DESC_IDX_NEXT_DESC) =
                PLX_LE_DATA_32(
                    (pDma->LocalToPciDma << 3) | (1 << 1) | (1 << 0)
                    );
        }
        else
        {
            // Calculate address of next descriptor
            BusSgl += SizeDescr;

            // Write next descriptor address
            *(((U32*)VaSgl) + SGL_DESC_IDX_NEXT_DESC) =
                PLX_LE_DATA_32(
                    BusSgl | (pDma->LocalToPciDma << 3) | (1 << 0)
                    );

            // Adjust Local address
            if (pdx->DmaInfo[channel].bLocalAddrConstant == FALSE)
                LocalAddr += BlockSize;

            // Adjust virtual address to next descriptor
            VaSgl += SizeDescr;

            // Clear offset
            offset = 0;
        }

        // Go to next page
        pCurrentPage++;
    }

    // Make sure cache data is flushed
    KeFlushIoBuffers(
        pMdl,
        (BOOLEAN)pDma->LocalToPciDma,   // Read from device operation?
        TRUE                            // DMA transfer?
        );

    // Return the physical address of the SGL
    *pSglAddress = BusSglOriginal;

    return ApiSuccess;
}


#endif  // PLX_DMA_SUPPORT

void FreeUserStruct( DEVICE_EXTENSION *dx)
{
	int ii;
	DebugPrint("Free User Struct");

	dx->bUserShared = FALSE;

	for(ii=0;ii<10;ii++) {
		if(dx->mdlAxis[ii]) {
			MmUnlockPages(dx->mdlAxis[ii]);
			IoFreeMdl(dx->mdlAxis[ii]);
			dx->mdlAxis[ii]=NULL;
			dx->pAxis[ii]=NULL;
		}
	}

	if(dx->mdl) {
		MmUnlockPages(dx->mdl);
		IoFreeMdl(dx->mdl);
		dx->mdl=NULL;
		dx->pSi10User=NULL;
	}

}

void FreeDataArray( DEVICE_EXTENSION *dx)
{
	if(dx->mdlDataArray) {
		MmUnlockPages(dx->mdlDataArray);
		IoFreeMdl(dx->mdlDataArray);
		dx->mdlDataArray=NULL;
		dx->pDataArray=NULL;
		DebugPrintMsg("Free Data Array Mdl");
	}
}


void IrqCallbackRoutine( IN PVOID Context)
{

}

void FreeTimeSlot( DEVICE_EXTENSION *dx)
{
	if(dx->mdlTimeSlot) {
		MmUnlockPages(dx->mdlTimeSlot);
		IoFreeMdl(dx->mdlTimeSlot);
		dx->mdlTimeSlot=NULL;
		dx->pTimeSlotUser=NULL;
		DebugPrintMsg("Free mdlTimeSlot");
	}

	dx->bTimeSlotShared = FALSE;

}

void FreeUtUserStruct( DEVICE_EXTENSION *dx)
{
	int ii;

	if(dx->utmdl) {
		MmUnlockPages(dx->utmdl);
		IoFreeMdl(dx->utmdl);
		dx->utmdl=NULL;
		dx->pUtUser=NULL;
		DebugPrintMsg("Free Struct UtMdl");
	}

	dx->bUTShared = FALSE;

	for(ii=0;ii<100;ii++){

		if(dx->mdlAmpBuffer[ii]) {
			MmUnlockPages(dx->mdlAmpBuffer[ii]);
			IoFreeMdl(dx->mdlAmpBuffer[ii]);
			dx->mdlAmpBuffer[ii]=NULL;
			dx->pAmpBuffer[ii]=NULL;
			DebugPrintMsg("Free Data Array Mdl");
		}

		if(dx->mdlTimeBuffer[ii]) {
			MmUnlockPages(dx->mdlTimeBuffer[ii]);
			IoFreeMdl(dx->mdlTimeBuffer[ii]);
			dx->mdlTimeBuffer[ii]=NULL;
			dx->pTimeBuffer[ii]=NULL;
			DebugPrintMsg("Free Data Array Mdl");
		}

	}

}

void DeRegisterEvent( DEVICE_EXTENSION *dx)
{

	if (dx->pIntEvent)
	{						// event already registered
		ObDereferenceObject(dx->pIntEvent);
		dx->pIntEvent = NULL;
	}

}

void WriteDWord( DEVICE_EXTENSION *pdx, unsigned long offset, unsigned long dWord)
{

	U8                BarIndex = 2;
    U32               *pBuffer = &dWord;
    U32               ByteCount = 4;
    PLX_ACCESS_TYPE   AccessType = (PLX_ACCESS_TYPE) BitSize32;
    BOOLEAN           bRemap = TRUE;
    BOOLEAN           bReadOperation = FALSE;

    U16          Offset_RegRemap;
    U32          RegValue;
    U32          SpaceRange;
    U32          SpaceOffset;
    U32          RemapOriginal;
    U32          BytesToTransfer;
    PLX_UINT_PTR SpaceVa;



    // Added to prevent compiler warnings
    RemapOriginal = 0;


    // Get offset of remap register
    PlxChipGetRemapOffset(
        pdx,
        BarIndex,
        &Offset_RegRemap
        );

    if (Offset_RegRemap == (U16)-1)
    {
        return;
    }

    // Only memory spaces are supported by this function
    if (pdx->PciBar[BarIndex].Properties.bIoSpace)
    {

        return;
    }

    // Get kernel virtual address for the space
    SpaceVa = (PLX_UINT_PTR)pdx->PciBar[BarIndex].pVa;

    if (SpaceVa == 0)
    {
       

        return;
    }

    // Save the remap register
    if (bRemap)
    {
        RemapOriginal =
            PLX_9000_REG_READ(
                pdx,
                Offset_RegRemap
                );
    }
    else
    {
        // Make sure requested area doesn't exceed our local space window boundary
        if ((offset + ByteCount) > (U32)pdx->PciBar[BarIndex].Properties.Size)
        {
         
            return;
        }
    }

    // Get the range of the space
    SpaceRange = ~((U32)pdx->PciBar[BarIndex].Properties.Size - 1);

    // Transfer data in blocks
    while (ByteCount != 0)
    {
        // Adjust remap if necessary
        if (bRemap)
        {
            // Clear upper bits of remap
            RegValue = RemapOriginal & ~SpaceRange;

            // Adjust window to local address
            RegValue |= offset & SpaceRange;

            PLX_9000_REG_WRITE(
                pdx,
                Offset_RegRemap,
                RegValue
                );
        }

        // Get current offset into space
        SpaceOffset = offset & (~SpaceRange);

        // Calculate bytes to transfer for next block
        if (ByteCount <= (((~SpaceRange) + 1) - SpaceOffset))
        {
            BytesToTransfer = ByteCount;
        }
        else
        {
            BytesToTransfer = ((~SpaceRange) + 1) - SpaceOffset;
        }

        if (bReadOperation)
        {
           
			  DEV_MEM_TO_USER_32(
				pBuffer,
				(SpaceVa + SpaceOffset),
				BytesToTransfer
				);
               
        }
        else
        {
           
			   USER_TO_DEV_MEM_32(
                (SpaceVa + SpaceOffset),
                pBuffer,
                BytesToTransfer
                );
        
        }

        // Adjust for next block access
        pBuffer   += BytesToTransfer;
        offset    += BytesToTransfer;
        ByteCount -= BytesToTransfer;
    }

    // Restore the remap register
    if (bRemap)
    {
        PLX_9000_REG_WRITE(
            pdx,
            Offset_RegRemap,
            RemapOriginal
            );
    }

    return;

}