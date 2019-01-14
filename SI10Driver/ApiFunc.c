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

/*******************************************************************************
 *
 * File Name:
 *
 *      ApiFunc.c
 *
 * Description:
 *
 *      Implements the PLX API functions
 *
 * Revision History:
 *
 *      07-01-08 : PLX SDK v6.00
 *
 ******************************************************************************/


#include "ApiFunc.h"
#include "Eep_9000.h"
#include "GlobalVars.h"
#include "PciFunc.h"
#include "PlxChipApi.h"
#include "PlxChipFn.h"
#include "PlxInterrupt.h"
#include "SuppFunc.h"
#include "DebugPrint.h"




/*******************************************************************************
 *
 * Function   :  PlxDeviceFind
 *
 * Description:  Search for a specific device using device key parameters
 *
 ******************************************************************************/
PLX_STATUS
PlxDeviceFind(
    DEVICE_EXTENSION *pdx,
    PLX_DEVICE_KEY   *pKey,
    U8               *pDeviceNumber
    )
{
    U8             DeviceCount;
    BOOLEAN        bMatchId;
    BOOLEAN        bMatchLoc;
    DEVICE_OBJECT  *fdo;


    DeviceCount = 0;

    // Get first device instance in list
    fdo = pdx->pDeviceObject->DriverObject->DeviceObject;

    // Compare with items in device list
    while (fdo != NULL)
    {
        // Get the device extension
        pdx = fdo->DeviceExtension;

        // Assume successful match
        bMatchLoc = TRUE;
        bMatchId  = TRUE;

        //
        // Compare device key information
        //

        // Compare Bus number
        if (pKey->bus != (U8)PCI_FIELD_IGNORE)
        {
            if (pKey->bus != pdx->Key.bus)
            {
                bMatchLoc = FALSE;
            }
        }

        // Compare Slot number
        if (pKey->slot != (U8)PCI_FIELD_IGNORE)
        {
            if (pKey->slot != pdx->Key.slot)
            {
                bMatchLoc = FALSE;
            }
        }

        // Compare Function number
        if (pKey->function != (U8)PCI_FIELD_IGNORE)
        {
            if (pKey->function != pdx->Key.function)
            {
                bMatchLoc = FALSE;
            }
        }

        //
        // Compare device ID information
        //

        // Compare Vendor ID
        if (pKey->VendorId != (U16)PCI_FIELD_IGNORE)
        {
            if (pKey->VendorId != pdx->Key.VendorId)
            {
                bMatchId = FALSE;
            }
        }

        // Compare Device ID
        if (pKey->DeviceId != (U16)PCI_FIELD_IGNORE)
        {
            if (pKey->DeviceId != pdx->Key.DeviceId)
            {
                bMatchId = FALSE;
            }
        }

        // Compare Subsystem Vendor ID
        if (pKey->SubVendorId != (U16)PCI_FIELD_IGNORE)
        {
            if (pKey->SubVendorId != pdx->Key.SubVendorId)
            {
                bMatchId = FALSE;
            }
        }

        // Compare Subsystem Device ID
        if (pKey->SubDeviceId != (U16)PCI_FIELD_IGNORE)
        {
            if (pKey->SubDeviceId != pdx->Key.SubDeviceId)
            {
                bMatchId = FALSE;
            }
        }

        // Compare Revision
        if (pKey->Revision != (U8)PCI_FIELD_IGNORE)
        {
            if (pKey->Revision != pdx->Key.Revision)
            {
                bMatchId = FALSE;
            }
        }

        // Check if match on location and ID
        if (bMatchLoc && bMatchId)
        {
            // Match found, check if it is the desired device
            if (DeviceCount == *pDeviceNumber)
            {
                // Copy the device information
                *pKey = pdx->Key; 

                DebugPrint(
                    "Criteria matched device %04X_%04X [b:%02x s:%02x f:%02x]\n",
                    pdx->Key.DeviceId, pdx->Key.VendorId,
                    pdx->Key.bus, pdx->Key.slot, pdx->Key.function
                    );

                return ApiSuccess;
            }

            // Increment device count
            DeviceCount++;
        }

        // Jump to next entry
        fdo = fdo->NextDevice;
    }

    // Return number of matched devices
    *pDeviceNumber = DeviceCount;

    DebugPrint("Criteria did not match any devices\n");

    return ApiInvalidDeviceInfo;
}




/*******************************************************************************
 *
 * Function   :  PlxChipTypeGet
 *
 * Description:  Returns PLX chip type and revision
 *
 ******************************************************************************/
PLX_STATUS
PlxChipTypeGet(
    DEVICE_EXTENSION *pdx,
    PLX_DEVICE_KEY   *pKey,
    U32              *pChipType,
    U8               *pRevision
    )
{
    U32 RegValue;


    // Reset values
    *pChipType = PLX_CHIP_TYPE;
    *pRevision = pKey->Revision;

    switch (*pChipType)
    {
        case 0x9080:
        case 0x9054:
        case 0x9056:
        case 0x9656:
            // Verify by reading hard-coded ID
            RegValue =
                PLX_9000_REG_READ(
                    pdx,
                    0x70               // Hard-coded ID register
                    );

            if (((RegValue & 0xFFFF) == 0x10b5) &&
                ((RegValue >> 16) == *pChipType))
            {
                // Get revision
                RegValue =
                    PLX_9000_REG_READ(
                        pdx,
                        0x74           // Revision ID
                        );

                // 9054 requires additional verification
                if (*pChipType != 0x9054)
                {
                    *pRevision = (U8)RegValue;
                }
                else
                {
                    // AA & AB versions have same revision ID
                    if (RegValue == 0xA)
                    {
                        PLX_PCI_REG_READ(
                            pdx,
                            CFG_REV_ID,
                            &RegValue
                            );

                        if ((RegValue & 0xf) == 0xb)
                            *pRevision = 0xAB;
                        else
                            *pRevision = 0xAA;
                    }
                    else
                    {
                        if (RegValue == 0xC)
                        {
                            // Set value for AC revision
                            *pRevision = 0xAC;
                        }
                        else
                        {
                            *pRevision = (U8)RegValue;
                        }
                    }
                }
            }
            break;

        case 0x9050:
        case 0x9030:
            // 9050/52 require additional verification
            if (*pChipType == 0x9030)
            {
                *pRevision = 0xAA;
            }
            else
            {
                // Read 9050/9052 PCI revision
                PLX_PCI_REG_READ(
                    pdx,
                    CFG_REV_ID,
                    &RegValue
                    );

                if ((RegValue & 0xF) == 0x2)
                {
                    *pRevision = 2;
                }
                else
                {
                    *pRevision = 1;
                }
            }
            break;

        case 0x8311:
            // Only AA revision exists
            *pRevision = 0xAA;
            break;

        default:
            DebugPrint("ERROR - Unable to determine chip type\n");
            return ApiInvalidDeviceInfo;
    } 

    DebugPrint(
        "Device %04X_%04X = %04X rev %02X\n",
        pKey->DeviceId, pKey->VendorId, *pChipType, *pRevision
        );

    return ApiSuccess;
}




/*******************************************************************************
 *
 * Function   :  PlxChipTypeSet
 *
 * Description:  Sets the PLX chip type dynamically
 *
 ******************************************************************************/
PLX_STATUS
PlxChipTypeSet(
    DEVICE_EXTENSION *pdx,
    PLX_DEVICE_KEY   *pKey,
    U32               ChipType,
    U8                Revision
    )
{
    // Setting the PLX chip type is not supported in this PnP driver
    return ApiUnsupportedFunction;
}




/*******************************************************************************
 *
 * Function   :  PlxGetPortProperties
 *
 * Description:  Returns the current port information and status
 *
 ******************************************************************************/
PLX_STATUS
PlxGetPortProperties(
    DEVICE_EXTENSION *pdx,
    PLX_DEVICE_KEY   *pKey,
    PLX_PORT_PROP    *pPortProp
    )
{
    DebugPrint("Device does not support PCI Express Capability\n");

    // Set default value for properties
    RtlZeroMemory(pPortProp, sizeof(PLX_PORT_PROP));

    // Mark device as non-PCIe
    pPortProp->bNonPcieDevice = TRUE;

    // Default to a legacy endpoint
    pPortProp->PortType = PLX_PORT_LEGACY_ENDPOINT;

    return ApiSuccess;
}




/*******************************************************************************
 *
 * Function   :  PlxPciDeviceReset
 *
 * Description:  Resets a device using software reset feature of PLX chip
 *
 ******************************************************************************/
PLX_STATUS
PlxPciDeviceReset(
    DEVICE_EXTENSION *pdx,
    PLX_DEVICE_KEY   *pKey
    )
{
    // Call chip-specific function
    return PlxChip_BoardReset(
        pdx
        );
}




/*******************************************************************************
 *
 * Function   :  PlxRegisterRead
 *
 * Description:  Reads a PLX-specific control register
 *
 ******************************************************************************/
U32
PlxRegisterRead(
    DEVICE_EXTENSION *pdx,
    PLX_DEVICE_KEY   *pKey,
    U32               offset,
    PLX_STATUS       *pStatus,
    BOOLEAN           bAdjustForPort
    )
{
    // Verify register offset
    if ((offset & 0x3) || (offset >= MAX_PLX_REG_OFFSET))
    {
        DebugPrint("ERROR - Invalid register offset (0x%x)\n", offset);

        if (pStatus != NULL)
            *pStatus = ApiInvalidOffset;

        return 0;
    }

    if (pStatus != NULL)
        *pStatus = ApiSuccess;

    return PLX_9000_REG_READ(
               pdx,
               offset
               );
}




/*******************************************************************************
 *
 * Function   :  PlxRegisterWrite
 *
 * Description:  Writes to a PLX-specific control register
 *
 ******************************************************************************/
PLX_STATUS
PlxRegisterWrite(
    DEVICE_EXTENSION *pdx,
    PLX_DEVICE_KEY   *pKey,
    U32               offset,
    U32               value,
    BOOLEAN           bAdjustForPort
    )
{
    // Verify register offset
    if ((offset & 0x3) || (offset >= MAX_PLX_REG_OFFSET))
    {
        DebugPrint("ERROR - Invalid register offset (0x%x)\n", offset);
        return ApiInvalidOffset;
    }

    PLX_9000_REG_WRITE(
        pdx,
        offset,
        value
        );

    return ApiSuccess;
}




/*******************************************************************************
 *
 * Function   :  PlxPciBarProperties
 *
 * Description:  Returns the properties of a PCI BAR space
 *
 ******************************************************************************/
PLX_STATUS
PlxPciBarProperties(
    DEVICE_EXTENSION *pdx,
    PLX_DEVICE_KEY   *pKey,
    U8                BarIndex,
    PLX_PCI_BAR_PROP *pBarProperties
    )
{
    // Verify BAR number
    switch (BarIndex)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            break;

        default:
            return ApiInvalidIndex;
    }

    // Get the actual BAR value in case OS didn't provide it
    if ((pdx->PciBar[BarIndex].Properties.BarValue == 0) &&
        (pdx->PciBar[BarIndex].Properties.Size != 0))
    {
        PLX_PCI_REG_READ(
            pdx,
            0x10 + (BarIndex * sizeof(U32)),
            &pdx->PciBar[BarIndex].Properties.BarValue
            );
    }

    // Return PCI BAR properties
    *pBarProperties = pdx->PciBar[BarIndex].Properties;

    // Display BAR properties
    DebugPrint(
        "    PCI BAR %d: %08X\n",
        BarIndex, pdx->PciBar[BarIndex].Properties.BarValue
        );

    DebugPrint(
        "    Phys Addr: %08lX\n",
        (PLX_UINT_PTR)pdx->PciBar[BarIndex].Properties.Physical
        );

    if (pdx->PciBar[BarIndex].Properties.Size >= (1 << 10))
    {
        DebugPrint(
            "    Size     : %08lx  (%ld Kb)\n",
            (PLX_UINT_PTR)pdx->PciBar[BarIndex].Properties.Size,
            (PLX_UINT_PTR)pdx->PciBar[BarIndex].Properties.Size >> 10
            );
    }
    else
    {
        DebugPrint(
            "    Size     : %08lx  (%ld bytes)\n",
            (PLX_UINT_PTR)pdx->PciBar[BarIndex].Properties.Size,
            (PLX_UINT_PTR)pdx->PciBar[BarIndex].Properties.Size
            );
    }

    DebugPrint(
        "    Prefetch?: %s\n",
        (pdx->PciBar[BarIndex].Properties.bPrefetchable) ? "Yes" : "No"
        );

    return ApiSuccess;
}




/*******************************************************************************
 *
 * Function   :  PlxPciBarMap
 *
 * Description:  Map a PCI BAR Space into User virtual space
 *
 ******************************************************************************/
PLX_STATUS
PlxPciBarMap(
    DEVICE_EXTENSION *pdx,
    PLX_DEVICE_KEY   *pKey,
    U8                BarIndex,
    VOID             *pUserVa,
    VOID             *pOwner
    )
{
    U8                   i;
    PMDL                 pMdl[MAX_VIRTUAL_ADDR];
    VOID                *Va[MAX_VIRTUAL_ADDR];
    VOID                *VaBase;
    BOOLEAN              bMapError;
    PLX_USER_MAPPING    *pMapObject;
    MEMORY_CACHING_TYPE  CacheMode;


    // Set default address
    *(PLX_UINT_PTR*)pUserVa = 0;

    // Verify BAR is of type memory
    if (pdx->PciBar[BarIndex].Properties.bIoSpace)
    {
        DebugPrint(
            "ERROR - BAR %d is an I/O space, cannot map to user space\n",
            BarIndex
            );

        return ApiInvalidPciSpace;
    }

    // Check if the space is valid
    if (pdx->PciBar[BarIndex].Properties.Physical == 0)
    {
        DebugPrint(
            "ERROR - BAR %d address (0x%08lx) is invalid\n",
            BarIndex, (PLX_UINT_PTR)pdx->PciBar[BarIndex].Properties.Physical
            );

        return ApiInvalidAddress;
    }

    // Verify Kernel virtual address
    if (pdx->PciBar[BarIndex].pVa == NULL)
    {
        DebugPrint(
            "ERROR - Kernel Virtual address for BAR %d is not valid\n",
            BarIndex
            );

        return ApiInvalidAddress;
    }

    // Make sure memory size is valid
    if (pdx->PciBar[BarIndex].Properties.Size == 0)
    {
        DebugPrint(
            "ERROR - Size of BAR %d is 0\n",
            BarIndex
            );

        return ApiInvalidSize;
    }

    // Verify the MDL
    if (pdx->PciBar[BarIndex].pMdl == NULL)
    {
        DebugPrint(
            "ERROR - MDL does not exist for BAR %d\n",
            BarIndex
            );

        return ApiInsufficientResources;
    }

    // Make sure there is enough space to store all virtual addresses
    if (pdx->PciBar[BarIndex].Properties.Size > (MAX_VIRTUAL_ADDR * MAX_MDL_SIZE))
    {
        DebugPrint(
            "ERROR - Not enough virtual addresses for space of size %ldMB\n",
            ((PLX_UINT_PTR)pdx->PciBar[BarIndex].Properties.Size >> 20)
            );

        return ApiInsufficientResources;
    }

    // Set initial values
    i         = 0;
    pMdl[0]   = pdx->PciBar[BarIndex].pMdl;
    VaBase    = NULL;
    bMapError = FALSE;

    /***********************************************************
     * For pre-fetchable region, enable write-combining
     * Cached mode seems to have no effect, but write-combining
     * results in much better CPU write performance.
     **********************************************************/
    if (pdx->PciBar[BarIndex].Properties.bPrefetchable)
        CacheMode = MmWriteCombined;
    else
        CacheMode = MmNonCached;

    // Clear the virtual addresses
    RtlZeroMemory(
        Va,
        MAX_VIRTUAL_ADDR * sizeof(VOID*)
        );

    // Attempt to map the BAR into user space
    while ((i < MAX_VIRTUAL_ADDR) && (pMdl[i] != NULL))
    {
        try
        {
            // Attempt to map next region
            Va[i] =
                MmMapLockedPagesSpecifyCache(
                    pMdl[i],             // MDL for region
                    UserMode,            // User or kernel mode?
                    CacheMode,           // Cache type?
                    VaBase,              // User address to use
                    FALSE,               // Do not issue a bug check (KernelMode only)
                    NormalPagePriority   // Priority of success
                    );
        }
        except (EXCEPTION_EXECUTE_HANDLER)
        {
            DebugPrint("EXCEPTION: Raised by MmMapLockedPagesSpecifyCache() for MDL #%d\n", i);
            Va[i] = NULL;
        }

        // Check if the mapping succeeded
        if (Va[i] == NULL)
        {
            DebugPrint(
                "ERROR - Unable to map PCI BAR %d (0x%08lx) ==> User space\n",
                BarIndex, (PLX_UINT_PTR)pdx->PciBar[BarIndex].Properties.Physical
                );

            bMapError = TRUE;
        }
        else if (VaBase != NULL)
        {
            // Make sure VA address is consecutive
            if (VaBase != Va[i])
            {
                DebugPrint(
                    "ERROR - Unable to assign contiguous virtual addresses for PCI BAR %d (0x%08lx) ==> User space\n",
                    BarIndex, (PLX_UINT_PTR)pdx->PciBar[BarIndex].Properties.Physical
                    );

                bMapError = TRUE;
            }
        }

        if (bMapError)
        {
            // Release any previous mappings
            while (i != 0)
            {
                i--;

                MmUnmapLockedPages(
                    Va[i],
                    pMdl[i]
                    );
            }

            return ApiInsufficientResources;
        }

        // Adjust to next consecutive base address
        VaBase = ((U8*)Va[i]) + MmGetMdlByteCount( pMdl[i] );

        // Go to next entry
        i++;
        pMdl[i] = pMdl[i-1]->Next;
    }

    // Return initial base address
    *(PLX_UINT_PTR*)pUserVa = (PLX_UINT_PTR)Va[0];

    DebugPrint(
        "Mapped PCI BAR %d (0x%08lx) ==> User VA (0x%08lx) (owner=%p)\n",
        BarIndex, (PLX_UINT_PTR)pdx->PciBar[BarIndex].Properties.Physical,
        *(PLX_UINT_PTR*)pUserVa, pOwner
        );

    // Add mapping to list for later unmapping
    pMapObject =
        ExAllocatePoolWithTag(
            NonPagedPool,
            sizeof(PLX_USER_MAPPING),
            '_XLP'        // Tag = "PLX_" in reverse order
            );

    if (pMapObject == NULL)
    {
        DebugPrint("ERROR: Unable to save mapping, insufficient memory\n");
    }
    else
    {
        // Record mapping properties
        pMapObject->pOwner   = pOwner;
        pMapObject->BarIndex = BarIndex;

        // Record the virtual address
        RtlCopyMemory(
            pMapObject->pUserVa,
            Va,
            MAX_VIRTUAL_ADDR * sizeof(VOID*)
            );

        // Save the mapping for later cleanup
        ExInterlockedInsertTailList(
            &(pdx->List_BarMappings),
            &(pMapObject->ListEntry),
            &(pdx->Lock_BarMappingsList)
            );
    }

    return ApiSuccess;
}




/*******************************************************************************
 *
 * Function   :  PlxPciBarUnmap
 *
 * Description:  Unmap a previously mapped PCI BAR Space from User virtual space
 *
 ******************************************************************************/
PLX_STATUS
PlxPciBarUnmap(
    DEVICE_EXTENSION *pdx,
    PLX_DEVICE_KEY   *pKey,
    VOID             *UserVa,
    VOID             *pOwner
    )
{
    U8                i;
    PMDL              pMdl;
    KIRQL             IrqL_Original;
    PLIST_ENTRY       pEntry;
    PLX_USER_MAPPING *pMapObject;


    // Find the mapped object
    KeAcquireSpinLock(
        &(pdx->Lock_BarMappingsList),
        &IrqL_Original
        );

    pEntry = pdx->List_BarMappings.Flink;

    while (pEntry != &(pdx->List_BarMappings))
    {
        pMapObject =
            CONTAINING_RECORD(
                pEntry,
                PLX_USER_MAPPING,
                ListEntry
                );

        if ((pMapObject->pOwner == pOwner) &&
            (pMapObject->pUserVa[0] == UserVa))
        {
            // Remove map object from list
            RemoveEntryList(
                pEntry
                );

            KeReleaseSpinLock(
                &(pdx->Lock_BarMappingsList),
                IrqL_Original
                );

            DebugPrint(
                "Unmapping User VA (0x%p) for BAR %d (owner=%p)...\n",
                pMapObject->pUserVa[0], pMapObject->BarIndex, pOwner
                );

            // Unmap the space
            i    = 0;
            pMdl = pdx->PciBar[pMapObject->BarIndex].pMdl;

            while ((i < MAX_VIRTUAL_ADDR) && (pMapObject->pUserVa[i] != NULL))
            {
                MmUnmapLockedPages(
                    pMapObject->pUserVa[i],
                    pMdl
                    );

                i++;
                pMdl = pMdl->Next;
            }

            // Release the list object
            ExFreePool(
                pMapObject
                );

            return ApiSuccess;
        }

        // Jump to next item
        pEntry = pEntry->Flink;
    }

    KeReleaseSpinLock(
        &(pdx->Lock_BarMappingsList),
        IrqL_Original
        );

    DebugPrint(
        "ERROR - Map object not found in list...\n"
        );

    return ApiInvalidAddress;
}




/*******************************************************************************
 *
 * Function   :  PlxEepromPresent
 *
 * Description:  Returns the state of the EEPROM as reported by the PLX device
 *
 ******************************************************************************/
PLX_STATUS
PlxEepromPresent(
    DEVICE_EXTENSION  *pdx,
    PLX_DEVICE_KEY    *pKey,
    PLX_EEPROM_STATUS *pStatus
    )
{
    return Plx9000_EepromPresent(
               pdx,
               pStatus
               );
}




/*******************************************************************************
 *
 * Function   :  PlxEepromProbe
 *
 * Description:  Probes for the presence of an EEPROM
 *
 ******************************************************************************/
PLX_STATUS
PlxEepromProbe(
    DEVICE_EXTENSION *pdx,
    PLX_DEVICE_KEY   *pKey,
    BOOLEAN          *pFlag
    )
{
    U16        OffsetProbe;
    U32        ValueRead;
    U32        ValueWrite;
    U32        ValueOriginal;
    PLX_STATUS rc;


    // Default to no EEPROM present
    *pFlag = FALSE;

    // Set EEPROM offset to use for probe
    OffsetProbe = 0x70;

    DebugPrint("Probing EEPROM at offset %02xh\n", OffsetProbe);

    // Get the current value
    rc =
        PlxEepromReadByOffset(
            pdx,
            pKey,
            OffsetProbe,
            &ValueOriginal
            );

    if (rc != ApiSuccess)
        return rc;

    // Prepare inverse value to write
    ValueWrite = ~(ValueOriginal);

    // Write inverse of original value
    rc =
        PlxEepromWriteByOffset(
            pdx,
            pKey,
            OffsetProbe,
            ValueWrite
            );

    if (rc != ApiSuccess)
        return rc;

    // Read updated value
    rc =
        PlxEepromReadByOffset(
            pdx,
            pKey,
            OffsetProbe,
            &ValueRead
            );

    if (rc != ApiSuccess)
        return rc;

    // Check if value was written properly
    if (ValueRead == ValueWrite)
    {
        DebugPrint("Probe detected an EEPROM present\n");

        *pFlag = TRUE;

        // Restore the original value
        PlxEepromWriteByOffset(
            pdx,
            pKey,
            OffsetProbe,
            ValueOriginal
            );
    }
    else
    {
        DebugPrint("Probe did not detect an EEPROM\n");
    }

    return ApiSuccess;
}




/*******************************************************************************
 *
 * Function   :  PlxEepromCrcGet
 *
 * Description:  Returns the EEPROM CRC and its status
 *
 ******************************************************************************/
PLX_STATUS
PlxEepromCrcGet(
    DEVICE_EXTENSION *pdx,
    PLX_DEVICE_KEY   *pKey,
    U32              *pCrc,
    U8               *pCrcStatus
    )
{
    // Device does not support CRC
    return ApiUnsupportedFunction;
}




/*******************************************************************************
 *
 * Function   :  PlxEepromCrcUpdate
 *
 * Description:  Updates the EEPROM CRC
 *
 ******************************************************************************/
PLX_STATUS
PlxEepromCrcUpdate(
    DEVICE_EXTENSION *pdx,
    PLX_DEVICE_KEY   *pKey,
    U32              *pCrc,
    BOOLEAN           bUpdateEeprom
    )
{
    // Device does not support CRC
    return ApiUnsupportedFunction;
}




/*******************************************************************************
 *
 * Function   :  PlxEepromReadByOffset
 *
 * Description:  Read a 32-bit value from the EEPROM at a specified offset
 *
 ******************************************************************************/
PLX_STATUS
PlxEepromReadByOffset(
    DEVICE_EXTENSION *pdx,
    PLX_DEVICE_KEY   *pKey,
    U16               offset,
    U32              *pValue
    )
{
    // Make sure offset is aligned on 32-bit boundary
    if (offset & (3 << 0))
    {
        *pValue = 0;
        return ApiInvalidOffset;
    }

    // Call chip-specific function
    return PlxChip_EepromReadByOffset(
        pdx,
        offset,
        pValue
        );
}




/*******************************************************************************
 *
 * Function   :  PlxEepromWriteByOffset
 *
 * Description:  Write a 32-bit value to the EEPROM at a specified offset
 *
 ******************************************************************************/
PLX_STATUS
PlxEepromWriteByOffset(
    DEVICE_EXTENSION *pdx,
    PLX_DEVICE_KEY   *pKey,
    U16               offset,
    U32               value
    )
{
    // Make sure offset is aligned on 32-bit boundary
    if (offset & (3 << 0))
    {
        return ApiInvalidOffset;
    }

    // Call chip-specific function
    return PlxChip_EepromWriteByOffset(
        pdx,
        offset,
        value
        );
}




/*******************************************************************************
 *
 * Function   :  PlxEepromReadByOffset_16
 *
 * Description:  Read a 16-bit value from the EEPROM at a specified offset
 *
 ******************************************************************************/
PLX_STATUS
PlxEepromReadByOffset_16(
    DEVICE_EXTENSION *pdx,
    PLX_DEVICE_KEY   *pKey,
    U16               offset,
    U16              *pValue
    )
{
    U32        Value_32;
    PLX_STATUS rc;


    // Set default return value
    *pValue = 0;

    // Make sure offset is aligned on 16-bit boundary
    if (offset & (1 << 0))
    {
        return ApiInvalidOffset;
    }

    /******************************************
     * For devices that do not support 16-bit
     * EEPROM accesses, use 32-bit access
     *****************************************/

    // Get current 32-bit value
    rc =
        PlxEepromReadByOffset(
            pdx,
            pKey,
            (offset & ~0x3),
            &Value_32
            );

    if (rc != ApiSuccess)
        return rc;

    // Return desired 16-bit portion
    if ((offset & 0x3) == 2)
    {
        *pValue = (U16)(Value_32 >> 16);
    }
    else
    {
        *pValue = (U16)(Value_32);
    }

    return ApiSuccess;
}




/*******************************************************************************
 *
 * Function   :  PlxEepromWriteByOffset_16
 *
 * Description:  Write a 16-bit value to the EEPROM at a specified offset
 *
 ******************************************************************************/
PLX_STATUS
PlxEepromWriteByOffset_16(
    DEVICE_EXTENSION *pdx,
    PLX_DEVICE_KEY   *pKey,
    U16               offset,
    U16               value
    )
{
    U32        Value_32;
    PLX_STATUS rc;


    // Make sure offset is aligned on 16-bit boundary
    if (offset & (1 << 0))
    {
        return ApiInvalidOffset;
    }

    /******************************************
     * For devices that do not support 16-bit
     * EEPROM accesses, use 32-bit access
     *****************************************/

    // Get current 32-bit value
    rc =
        PlxEepromReadByOffset(
            pdx,
            pKey,
            (offset & ~0x3),
            &Value_32
            );

    if (rc != ApiSuccess)
        return rc;

    // Insert new 16-bit value in correct location
    if ((offset & 0x3) == 2)
    {
        Value_32 = ((U32)value << 16) | (Value_32 & 0xFFFF);
    }
    else
    {
        Value_32 = ((U32)value) | (Value_32 & 0xFFFF0000);
    }

    // Update EEPROM
    return PlxEepromWriteByOffset(
        pdx,
        pKey,
        (offset & ~0x3),
        Value_32
        );
}




/*******************************************************************************
 *
 * Function   :  PlxPciIoPortTransfer
 *
 * Description:  Read or Write from/to an I/O port
 *
 ******************************************************************************/
PLX_STATUS
PlxPciIoPortTransfer(
    U64              IoPort,
    VOID            *pBuffer,
    U32              SizeInBytes,
    PLX_ACCESS_TYPE  AccessType,
    BOOLEAN          bReadOperation
    )
{
    U8 AccessSize;


    if (pBuffer == NULL)
        return ApiNullParam;

    // Verify size & type
    switch (AccessType)
    {
        case BitSize8:
            AccessSize = sizeof(U8);
            break;

        case BitSize16:
            if (IoPort & (1 << 0))
            {
                DebugPrint("ERROR - I/O port not aligned on 16-bit boundary\n");
                return ApiInvalidAddress;
            }

            if (SizeInBytes & (1 << 0))
            {
                DebugPrint("ERROR - Byte count not aligned on 16-bit boundary\n");
                return ApiInvalidSize;
            }
            AccessSize = sizeof(U16);
            break;

        case BitSize32:
            if (IoPort & 0x3)
            {
                DebugPrint("ERROR - I/O port not aligned on 32-bit boundary\n");
                return ApiInvalidAddress;
            }

            if (SizeInBytes & 0x3)
            {
                DebugPrint("ERROR - Byte count not aligned on 32-bit boundary\n");
                return ApiInvalidSize;
            }
            AccessSize = sizeof(U32);
            break;

        default:
            return ApiInvalidAccessType;
    }

    while (SizeInBytes)
    {
        if (bReadOperation)
        {
            switch (AccessType)
            {
                case BitSize8:
                    *(U8*)pBuffer = IO_PORT_READ_8( IoPort );
                    break;

                case BitSize16:
                    *(U16*)pBuffer = IO_PORT_READ_16( IoPort );
                    break;

                case BitSize32:
                    *(U32*)pBuffer = IO_PORT_READ_32( IoPort );
                    break;

                default:
                    // Added to avoid compiler warnings
                    break;
            }
        }
        else
        {
            switch (AccessType)
            {
                case BitSize8:
                    IO_PORT_WRITE_8(
                        IoPort,
                        *(U8*)pBuffer
                        );
                    break;

                case BitSize16:
                    IO_PORT_WRITE_16(
                        IoPort,
                        *(U16*)pBuffer
                        );
                    break;

                case BitSize32:
                    IO_PORT_WRITE_32(
                        IoPort,
                        *(U32*)pBuffer
                        );
                    break;

                default:
                    // Added to avoid compiler warnings
                    break;
            }
        }

        // Adjust pointer & byte count
        pBuffer      = (VOID*)((PLX_UINT_PTR)pBuffer + AccessSize);
        SizeInBytes -= AccessSize;
    }

    return ApiSuccess;
}




/*******************************************************************************
 *
 * Function   :  PlxPciPhysicalMemoryAllocate
 *
 * Description:  Allocate physically contiguous page-locked memory
 *
 ******************************************************************************/
PLX_STATUS
PlxPciPhysicalMemoryAllocate(
    DEVICE_EXTENSION *pdx,
    PLX_PHYSICAL_MEM *pPciMem,
    BOOLEAN           bSmallerOk,
    VOID             *pOwner
    )
{
    U32                  SizeToMap;
    U32                  SizeRemain;
    U32                  DecrementAmount;
    VOID                *pVa;
    PMDL                 pMdl;
    PMDL                 pMdl_Previous;
    PLX_PHYS_MEM_OBJECT *pBufferObj;


    // Initialize buffer information
    pPciMem->UserAddr     = 0;
    pPciMem->PhysicalAddr = 0;
    pPciMem->CpuPhysical  = 0;

    /*******************************************************
     * Verify size
     *
     * A size of 0 is valid because this function may
     * be called to allocate a common buffer of size 0;
     * therefore, the information is reset & return sucess.
     ******************************************************/
    if (pPciMem->Size == 0)
    {
        return ApiSuccess;
    }

    // Allocate memory for new list object
    pBufferObj =
        ExAllocatePoolWithTag(
            NonPagedPool,
            sizeof(PLX_PHYS_MEM_OBJECT),
            '_XLP'        // Tag = "PLX_" in reverse order
            );

    if (pBufferObj == NULL)
    {
        DebugPrint("ERROR - Memory allocation for list object failed\n");
        return ApiInsufficientResources;
    }

    // Clear object
    RtlZeroMemory( pBufferObj, sizeof(PLX_PHYS_MEM_OBJECT) );

    // Set buffer request size
    pBufferObj->Size = pPciMem->Size;

    // Set Cacheability of the buffer. System RAM is always assumed as cacheable
    pBufferObj->bCacheable = MmCached;

    // Setup amount to reduce on failure
    DecrementAmount = (pPciMem->Size / 10);

    DebugPrint(
        "Attempting to allocate physical memory (%d Kb)...\n",
        (pPciMem->Size >> 10)
        );

    do
    {
        // Attempt to allocate the buffer
        pBufferObj->pKernelVa =
            Plx_dma_buffer_alloc(
                pdx,
                pBufferObj
                );

        if (pBufferObj->pKernelVa == NULL)
        {
            // Reduce memory request size if requested
            if (bSmallerOk && (pBufferObj->Size > PAGE_SIZE))
            {
                pBufferObj->Size -= DecrementAmount;
            }
            else
            {
                // Release the list object
                ExFreePool(
                    pBufferObj
                    );

                DebugPrint(
                    "ERROR - Physical memory allocation failed\n"
                    );

                pPciMem->Size = 0;

                return ApiInsufficientResources;
            }
        }
    }
    while (pBufferObj->pKernelVa == NULL);

    // Record buffer owner
    pBufferObj->pOwner = pOwner;

    /******************************************************************
     * A future mapping into user space will require an MDL. Due to
     * the limited region a single MDL can describe, multiple MDL's
     * must be allocated and chained together via the 'Next' field.
     *****************************************************************/

    // Set initial values
    pVa        = pBufferObj->pKernelVa;
    SizeRemain = (U32)pBufferObj->Size;

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
            DebugPrint("ERROR - MDL allocation for space failed\n");

            // Release any created MDLs
            while (pBufferObj->pMdl != NULL)
            {
                pMdl = pBufferObj->pMdl;

                pBufferObj->pMdl = pMdl->Next;

                IoFreeMdl( pMdl );
            }

            return STATUS_SUCCESS;
        }

        // Build the MDL
        MmBuildMdlForNonPagedPool( pMdl );

        // Clear next MDL pointer
        pMdl->Next = NULL;

        // Store MDL
        if (pBufferObj->pMdl == NULL)
        {
            // Insert first MDL
            pBufferObj->pMdl = pMdl;
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

    // Initialize mappings list
    InitializeListHead(
        &(pBufferObj->List_Mappings)
        );

    KeInitializeSpinLock(
        &(pBufferObj->Lock_MappingsList)
        );

    // Return buffer information
    pPciMem->Size         = pBufferObj->Size;
    pPciMem->PhysicalAddr = pBufferObj->BusPhysical;
    pPciMem->CpuPhysical  = pBufferObj->CpuPhysical;

    // Add buffer object to list
    ExInterlockedInsertTailList(
        &(pdx->List_PhysicalMem),
        &(pBufferObj->ListEntry),
        &(pdx->Lock_PhysicalMemList)
        );

    // Check if this is the common buffer allocation
    if (pdx == pOwner)
    {
        // Store common buffer information
        pGbl_CommonBuffer = pBufferObj;
    }

    return ApiSuccess;
}




/*******************************************************************************
 *
 * Function   :  PlxPciPhysicalMemoryFree
 *
 * Description:  Free previously allocated physically contiguous page-locked memory
 *
 ******************************************************************************/
PLX_STATUS
PlxPciPhysicalMemoryFree(
    DEVICE_EXTENSION *pdx,
    PLX_PHYSICAL_MEM *pPciMem
    )
{
    PMDL                 pMdl;
    KIRQL                IrqL_Original;
    PLIST_ENTRY          pEntry;
    PLX_PHYS_MEM_OBJECT *pBufferObj;


    KeAcquireSpinLock(
        &(pdx->Lock_PhysicalMemList),
        &IrqL_Original
        );

    pEntry = pdx->List_PhysicalMem.Flink;

    // Traverse list to find the desired list object
    while (pEntry != &(pdx->List_PhysicalMem))
    {
        // Get the object
        pBufferObj =
            CONTAINING_RECORD(
                pEntry,
                PLX_PHYS_MEM_OBJECT,
                ListEntry
                );

        // Check if the physical addresses matches
        if (pBufferObj->BusPhysical == pPciMem->PhysicalAddr)
        {
            // Release lock until mappings are removed
            KeReleaseSpinLock(
                &(pdx->Lock_PhysicalMemList),
                IrqL_Original
                );

            // Make sure all mappings to this memory are unmapped
            PlxPciPhysicalMemoryUnmapAll_ByOwner(
                pdx,
                pBufferObj,
                NULL         // Ignore owner to remove all mappings
                );

            // Remove the object from the list
            KeAcquireSpinLock(
                &(pdx->Lock_PhysicalMemList),
                &IrqL_Original
                );

            RemoveEntryList(
                pEntry
                );

            KeReleaseSpinLock(
                &(pdx->Lock_PhysicalMemList),
                IrqL_Original
                );

            // Release the MDLs describing the region
            while (pBufferObj->pMdl != NULL)
            {
                pMdl = pBufferObj->pMdl;

                pBufferObj->pMdl = pMdl->Next;

                IoFreeMdl( pMdl );
            }

            // Release the buffer
            Plx_dma_buffer_free(
                pdx,
                pBufferObj
                );

            // Release the list object
            ExFreePool(
                pBufferObj
                );

            // Check if this is the common buffer release
            if (pGbl_CommonBuffer == pBufferObj)
            {
                // Clear common buffer information
                pGbl_CommonBuffer = NULL;
            }

            return ApiSuccess;
        }

        // Jump to next item in the list
        pEntry = pEntry->Flink;
    }

    KeReleaseSpinLock(
        &(pdx->Lock_PhysicalMemList),
        IrqL_Original
        );

    DebugPrint(
        "ERROR - buffer object not found in list\n"
        );

    return ApiInvalidData;
}




/*******************************************************************************
 *
 * Function   :  PlxPciPhysicalMemoryMap
 *
 * Description:  Maps physical memory to User virtual address space
 *
 ******************************************************************************/
PLX_STATUS
PlxPciPhysicalMemoryMap(
    DEVICE_EXTENSION *pdx,
    PLX_PHYSICAL_MEM *pPciMem,
    VOID             *pOwner
    )
{
    U8                   i;
    PMDL                 pMdl[MAX_VIRTUAL_ADDR];
    VOID                *Va[MAX_VIRTUAL_ADDR];
    VOID                *VaBase;
    BOOLEAN              bMapError;
    KIRQL                IrqL_Original;
    BOOLEAN              bFound;
    PLIST_ENTRY          pEntry;
    PHYSICAL_ADDRESS     AddrPhysical;
    PLX_USER_MAPPING    *pMapObject;
    PLX_PHYS_MEM_OBJECT *pMemObject;


    // Set default return value
    pPciMem->UserAddr = 0;

    pMemObject = NULL;

    // Check if memory object is common buffer
    if (pGbl_CommonBuffer != NULL)
    {
        if (pPciMem->PhysicalAddr == pGbl_CommonBuffer->BusPhysical)
        {
            pMemObject = pGbl_CommonBuffer;
        }
    }

    // Find the memory object to map
    if (pMemObject == NULL)
    {
        // Find the object in the list
        KeAcquireSpinLock(
            &(pdx->Lock_PhysicalMemList),
            &IrqL_Original
            );

        pEntry = pdx->List_PhysicalMem.Flink;

        bFound = FALSE;

        // Traverse list to find the desired list object
        while (!bFound && (pEntry != &(pdx->List_PhysicalMem)))
        {
            // Get the object
            pMemObject =
                CONTAINING_RECORD(
                    pEntry,
                    PLX_PHYS_MEM_OBJECT,
                    ListEntry
                    );

            // Check if the physical addresses matches
            if (pMemObject->BusPhysical == pPciMem->PhysicalAddr)
            {
                bFound = TRUE;
            }
            else
            {
                // Jump to next item in the list
                pEntry = pEntry->Flink;
            }
        }

        KeReleaseSpinLock(
            &(pdx->Lock_PhysicalMemList),
            IrqL_Original
            );

        if (!bFound)
        {
            DebugPrint("ERROR - Physical memory object not found in list, unable to map\n");
            return ApiInvalidAddress;
        }
    }

    // Verify an MDL for the memory
    if (pMemObject->pMdl == NULL)
    {
        DebugPrint("ERROR - MDL does not exist for this memory, cannot map to user space\n");
        return ApiInsufficientResources;
    }

    // Make sure there is enough space to store all virtual addresses
    if (pMemObject->Size > (MAX_VIRTUAL_ADDR * MAX_MDL_SIZE))
    {
        DebugPrint(
            "ERROR - Not enough virtual addresses for memory of size %ldMB\n",
            (pMemObject->Size >> 20)
            );

        return ApiInsufficientResources;
    }

    // Set initial values
    i         = 0;
    pMdl[0]   = pMemObject->pMdl;
    VaBase    = NULL;
    bMapError = FALSE;

    // Clear the virtual addresses
    RtlZeroMemory(
        Va,
        MAX_VIRTUAL_ADDR * sizeof(VOID*)
        );

    // Attempt to map the BAR into user space
    while ((i < MAX_VIRTUAL_ADDR) && (pMdl[i] != NULL))
    {
        try
        {
            // Attempt to map next region
            Va[i] =
                MmMapLockedPagesSpecifyCache(
                    pMdl[i],             // MDL for region
                    UserMode,            // User or kernel mode?
                    MmCached,            // System RAM is always Cached (otherwise mapping fails)
                    VaBase,              // User address to use
                    FALSE,               // Do not issue a bug check (KernelMode only)
                    HighPagePriority     // Priority of success
                    );
        }
        except (EXCEPTION_EXECUTE_HANDLER)
        {
            DebugPrint("EXCEPTION: Raised by MmMapLockedPagesSpecifyCache() for MDL #%d\n", i);
            Va[i] = NULL;
        }

        // Check if the mapping succeeded
        if (Va[i] == NULL)
        {
            DebugPrint(
                "ERROR - Unable to map Physical address (0x%08lx) ==> User Space\n",
                (PLX_UINT_PTR)pMemObject->CpuPhysical
                );

            bMapError = TRUE;
        }
        else if (VaBase != NULL)
        {
            // Make sure VA address is consecutive
            if (VaBase != Va[i])
            {
                DebugPrint(
                    "ERROR - Unable to assign contiguous virtual addresses for memory (0x%08lx) ==> User space\n",
                    (PLX_UINT_PTR)pMemObject->CpuPhysical
                    );

                bMapError = TRUE;
            }
        }

        if (bMapError)
        {
            // Release any previous mappings
            while (i != 0)
            {
                i--;

                MmUnmapLockedPages(
                    Va[i],
                    pMdl[i]
                    );
            }

            return ApiInsufficientResources;
        }

        // Adjust to next consecutive base address
        VaBase = ((U8*)Va[i]) + MmGetMdlByteCount( pMdl[i] );

        // Go to next entry
        i++;
        pMdl[i] = pMdl[i-1]->Next;
    }

    DebugPrint(
        "Mapped Physical (0x%08lx) ==> VA (0x%p) (owner=%p)\n",
        (PLX_UINT_PTR)pMemObject->CpuPhysical, Va[0], pOwner
        );

    // Return virtual address
    pPciMem->UserAddr = (PLX_UINT_PTR)Va[0];

    // Save mapping to list for this physical memory
    pMapObject =
        ExAllocatePoolWithTag(
            NonPagedPool,
            sizeof(PLX_USER_MAPPING),
            '_XLP'        // Tag = "PLX_" in reverse order
            );

    if (pMapObject == NULL)
    {
        DebugPrint("ERROR: Unable to save mapping, insufficient memory\n");
    }
    else
    {
        // Record the IRP owner
        pMapObject->pOwner = pOwner;

        // Record the virtual address
        RtlCopyMemory(
            pMapObject->pUserVa,
            Va,
            MAX_VIRTUAL_ADDR * sizeof(VOID*)
            );

        // Save the mapping for later cleanup
        ExInterlockedInsertTailList(
            &(pMemObject->List_Mappings),
            &(pMapObject->ListEntry),
            &(pMemObject->Lock_MappingsList)
            );
    }

    return ApiSuccess;
}




/*******************************************************************************
 *
 * Function   :  PlxPciPhysicalMemoryUnmap
 *
 * Description:  Unmap physical memory from User virtual address space
 *
 ******************************************************************************/
PLX_STATUS
PlxPciPhysicalMemoryUnmap(
    DEVICE_EXTENSION *pdx,
    PLX_PHYSICAL_MEM *pPciMem,
    VOID             *pOwner
    )
{
    U8                   i;
    PMDL                 pMdl;
    VOID                *pUserVa;
    KIRQL                IrqL_Original;
    BOOLEAN              bFound;
    PLIST_ENTRY          pEntry;
    PHYSICAL_ADDRESS     AddrPhysical;
    PLX_USER_MAPPING    *pMapObject;
    PLX_PHYS_MEM_OBJECT *pMemObject;


    pMemObject = NULL;

    // Check if memory object is common buffer
    if (pGbl_CommonBuffer != NULL)
    {
        if (pPciMem->PhysicalAddr == pGbl_CommonBuffer->BusPhysical)
        {
            pMemObject = pGbl_CommonBuffer;
        }
    }

    // Find the memory object to unmap
    if (pMemObject == NULL)
    {
        // Find the object in the list
        KeAcquireSpinLock(
            &(pdx->Lock_PhysicalMemList),
            &IrqL_Original
            );

        pEntry = pdx->List_PhysicalMem.Flink;

        bFound = FALSE;

        // Traverse list to find the desired list object
        while (!bFound && (pEntry != &(pdx->List_PhysicalMem)))
        {
            // Get the object
            pMemObject =
                CONTAINING_RECORD(
                    pEntry,
                    PLX_PHYS_MEM_OBJECT,
                    ListEntry
                    );

            // Check if the physical addresses matches
            if (pMemObject->BusPhysical == pPciMem->PhysicalAddr)
            {
                bFound = TRUE;
            }
            else
            {
                // Jump to next item in the list
                pEntry = pEntry->Flink;
            }
        }

        KeReleaseSpinLock(
            &(pdx->Lock_PhysicalMemList),
            IrqL_Original
            );

        if (!bFound)
        {
            DebugPrint(
                "ERROR - Physical memory object not found in list, unable to unmap\n"
                );

            return ApiInvalidAddress;
        }
    }

    // Find the map object to unmap
    KeAcquireSpinLock(
        &(pMemObject->Lock_MappingsList),
        &IrqL_Original
        );

    pEntry = pMemObject->List_Mappings.Flink;

    bFound = FALSE;

    // Traverse list to find the desired list object
    while (!bFound && (pEntry != &(pMemObject->List_Mappings)))
    {
        // Get the object
        pMapObject =
            CONTAINING_RECORD(
                pEntry,
                PLX_USER_MAPPING,
                ListEntry
                );

        // Compare owner & virtual address
        if ((pOwner == pMapObject->pOwner) &&
            ((PLX_UINT_PTR)pMapObject->pUserVa[0] == pPciMem->UserAddr))
        {
            // Remove entry from the list
            RemoveEntryList(
                pEntry
                );

            bFound = TRUE;
        }
        else
        {
            // Jump to next item in the list
            pEntry = pEntry->Flink;
        }
    }

    KeReleaseSpinLock(
        &(pMemObject->Lock_MappingsList),
        IrqL_Original
        );

    if (!bFound)
    {
        DebugPrint("ERROR - Mapping object not found in list, unable to unmap\n");
        return ApiInvalidAddress;
    }

    // Unmap the memory
    if (pMapObject->pUserVa[0] == NULL)
    {
        DebugPrint(
            "ERROR - Invalid virtual address (0x%p), cannot unmap\n",
            pMapObject->pUserVa[0]
            );

        return ApiInvalidAddress;
    }

    DebugPrint(
        "Unmapping User VA (0x%p) for physical memory (owner=%p)...\n",
        pMapObject->pUserVa[0], pOwner
        );

    // Unmap memory from user space
    i    = 0;
    pMdl = pMemObject->pMdl;

    while ((i < MAX_VIRTUAL_ADDR) && (pMapObject->pUserVa[i] != NULL))
    {
        MmUnmapLockedPages(
            pMapObject->pUserVa[i],
            pMdl
            );

        i++;
        pMdl = pMdl->Next;
    }

    // Clear virtual address
    pPciMem->UserAddr = 0;

    // Release memory for map object
    ExFreePool(
        pMapObject
        );

    return ApiSuccess;
}




/*******************************************************************************
 *
 * Function   :  PlxInterruptEnable
 *
 * Description:  Enables specific interupts of the PLX Chip
 *
 ******************************************************************************/
PLX_STATUS
PlxInterruptEnable(
    DEVICE_EXTENSION *pdx,
    PLX_INTERRUPT    *pPlxIntr
    )
{
    // Call chip-specific function
    return PlxChip_InterruptEnable(
        pdx,
        pPlxIntr
        );
}




/*******************************************************************************
 *
 * Function   :  PlxInterruptDisable
 *
 * Description:  Disables specific interrupts of the PLX Chip
 *
 ******************************************************************************/
PLX_STATUS
PlxInterruptDisable(
    DEVICE_EXTENSION *pdx,
    PLX_INTERRUPT    *pPlxIntr
    )
{
    // Call chip-specific function
    return PlxChip_InterruptDisable(
        pdx,
        pPlxIntr
        );
}




/*******************************************************************************
 *
 * Function   :  PlxNotificationRegisterFor
 *
 * Description:  Registers a wait object for notification on interrupt(s)
 *
 ******************************************************************************/
PLX_STATUS
PlxNotificationRegisterFor(
    DEVICE_EXTENSION  *pdx,
    PLX_INTERRUPT     *pPlxIntr,
    PLX_NOTIFY_OBJECT *pEvent,
    VOID              *pOwner
    )
{
    PKEVENT          pKEvent;
    NTSTATUS         status;
    PLX_WAIT_OBJECT *pWaitObject;


    // Setup driver access to the user event handle
    status =
        ObReferenceObjectByHandle(
            (HANDLE)pEvent->hEvent, // Handle
            EVENT_MODIFY_STATE,     // Desired Access
            *ExEventObjectType,     // Object type
            KernelMode,             // Access Mode
            &pKEvent,               // Object pointer
            NULL                    // Handle information
            );

    if (status != STATUS_SUCCESS)
        return ApiInvalidHandle;

    // Allocate a new wait object
    pWaitObject =
        ExAllocatePoolWithTag(
            NonPagedPool,
            sizeof(PLX_WAIT_OBJECT),
            '_XLP'        // Tag = "PLX_" in reverse order
            );

    if (pWaitObject == NULL)
    {
        DebugPrint(
            "ERROR - memory allocation for interrupt wait object failed\n"
            );

        // De-reference the object
        ObDereferenceObject(
            pKEvent
            );

        return ApiInsufficientResources;
    }

    // Record the wait object
    pEvent->pWaitObject = (PLX_UINT_PTR)pWaitObject;

    // Record the owner
    pWaitObject->pOwner = pOwner;

    // Store kernel event handle
    pWaitObject->pKEvent = pKEvent;

    // Clear interrupt source
    pWaitObject->Source_Ints     = INTR_TYPE_NONE;
    pWaitObject->Source_Doorbell = 0;

    // Set interrupt notification flags
    PlxChipSetInterruptNotifyFlags(
        pPlxIntr,
        pWaitObject
        );

    // Add to list of waiting objects
    ExInterlockedInsertTailList(
        &(pdx->List_WaitObjects),
        &(pWaitObject->ListEntry),
        &(pdx->Lock_WaitObjectsList)
        );

    DebugPrint(
        "Registered interrupt wait object (%p)\n",
        pWaitObject
        );

    return ApiSuccess;
}




/*******************************************************************************
 *
 * Function   :  PlxNotificationWait
 *
 * Description:  Put the process to sleep until wake-up event occurs or timeout
 *
 ******************************************************************************/
PLX_STATUS
PlxNotificationWait(
    DEVICE_EXTENSION  *pdx,
    PLX_NOTIFY_OBJECT *pEvent,
    PLX_UINT_PTR       Timeout_ms
    )
{
    // Implemented at API level in Windows driver
    return ApiUnsupportedFunction;
}




/*******************************************************************************
 *
 * Function   :  PlxNotificationStatus
 *
 * Description:  Returns the interrupt(s) that have caused notification events
 *
 ******************************************************************************/
PLX_STATUS
PlxNotificationStatus(
    DEVICE_EXTENSION  *pdx,
    PLX_NOTIFY_OBJECT *pEvent,
    PLX_INTERRUPT     *pPlxIntr
    )
{
    KIRQL               IrqL_Original;
    PLIST_ENTRY         pEntry;
    PLX_WAIT_OBJECT    *pWaitObject;
    PLX_INTERRUPT_DATA  IntData;


    KeAcquireSpinLock(
        &(pdx->Lock_WaitObjectsList),
        &IrqL_Original
        );

    pEntry = pdx->List_WaitObjects.Flink;

    // Traverse list to find the desired list object
    while (pEntry != &(pdx->List_WaitObjects))
    {
        // Get the object
        pWaitObject =
            CONTAINING_RECORD(
                pEntry,
                PLX_WAIT_OBJECT,
                ListEntry
                );

        // Check if desired object
        if ((PLX_UINT_PTR)pWaitObject == pEvent->pWaitObject)
        {
            // Copy the interrupt sources
            IntData.Source_Ints     = pWaitObject->Source_Ints;
            IntData.Source_Doorbell = pWaitObject->Source_Doorbell;

            // Reset interrupt sources
            pWaitObject->Source_Ints     = INTR_TYPE_NONE;
            pWaitObject->Source_Doorbell = 0;

            KeReleaseSpinLock(
                &(pdx->Lock_WaitObjectsList),
                IrqL_Original
                );

            DebugPrint(
                "Returning status for interrupt wait object (%p)...\n",
                pWaitObject
                );

            // Set triggered interrupts
            PlxChipSetInterruptStatusFlags(
                &IntData,
                pPlxIntr
                );

            return ApiSuccess;
        }

        // Jump to next item in the list
        pEntry = pEntry->Flink;
    }

    KeReleaseSpinLock(
        &(pdx->Lock_WaitObjectsList),
        IrqL_Original
        );

    return ApiFailed;
}




/*******************************************************************************
 *
 * Function   :  PlxNotificationCancel
 *
 * Description:  Cancels a registered notification event
 *
 ******************************************************************************/
PLX_STATUS
PlxNotificationCancel(
    DEVICE_EXTENSION  *pdx,
    PLX_NOTIFY_OBJECT *pEvent,
    VOID              *pOwner
    )
{
    KIRQL            IrqL_Original;
    BOOLEAN          bRemove;
    PLIST_ENTRY      pEntry;
    PLX_WAIT_OBJECT *pWaitObject;


    KeAcquireSpinLock(
        &(pdx->Lock_WaitObjectsList),
        &IrqL_Original
        );

    pEntry = pdx->List_WaitObjects.Flink;

    // Find the object and remove it
    while (pEntry != &(pdx->List_WaitObjects))
    {
        // Get the object
        pWaitObject =
            CONTAINING_RECORD(
                pEntry,
                PLX_WAIT_OBJECT,
                ListEntry
                );

        // Default to not remove
        bRemove = FALSE;

        // Determine if object should be removed
        if (pOwner == pWaitObject->pOwner)
        {
            if (pEvent == NULL)
            {
                bRemove = TRUE;
            }
            else if ((PLX_UINT_PTR)pWaitObject == pEvent->pWaitObject)
            {
                bRemove = TRUE;
            }
        }

        // Remove object
        if (bRemove)
        {
            DebugPrint(
                "Removing interrupt wait object (%p)...\n",
                pWaitObject
                );

            // Remove the object from the list
            RemoveEntryList(
                pEntry
                );

            KeReleaseSpinLock(
                &(pdx->Lock_WaitObjectsList),
                IrqL_Original
                );

            // De-reference the kernel event object
            ObDereferenceObject(
                pWaitObject->pKEvent
                );

            // Release the list object
            ExFreePool(
                pWaitObject
                );

            // Return if removing only a specific object
            if (pEvent != NULL)
                return ApiSuccess;

            // Reset to beginning of list
            KeAcquireSpinLock(
                &(pdx->Lock_WaitObjectsList),
                &IrqL_Original
                );

            pEntry = pdx->List_WaitObjects.Flink;
        }
        else
        {
            // Jump to next item in the list
            pEntry = pEntry->Flink;
        }
    }

    KeReleaseSpinLock(
        &(pdx->Lock_WaitObjectsList),
        IrqL_Original
        );

    return ApiFailed;
}




/*******************************************************************************
 *
 * Function   :  PlxPciBarSpaceTransfer
 *
 * Description:  Accesses a PCI BAR space
 *
 ******************************************************************************/
PLX_STATUS
PlxPciBarSpaceTransfer(
    DEVICE_EXTENSION *pdx,
    U8                BarIndex,
    U32               offset,
    U8               *pBuffer,
    U32               ByteCount,
    PLX_ACCESS_TYPE   AccessType,
    BOOLEAN           bRemap,
    BOOLEAN           bReadOperation
    )
{
    U16          Offset_RegRemap;
    U32          RegValue;
    U32          SpaceRange;
    U32          SpaceOffset;
    U32          RemapOriginal;
    U32          BytesToTransfer;
    PLX_UINT_PTR SpaceVa;


    DebugPrint(
        "%s PCI BAR %d (%s=%x  %d bytes)\n",
        (bReadOperation) ? "Read from" : "Write to",
        BarIndex,
        (bRemap) ? "Local Addr" : "Offset",
        offset,
        ByteCount
        );

    // Added to prevent compiler warnings
    RemapOriginal = 0;

    // Verify data alignment
    switch (AccessType)
    {
        case BitSize8:
            break;

        case BitSize16:
            if (offset & 0x1)
            {
                DebugPrint("ERROR - Local address not aligned");
                return ApiInvalidAddress;
            }

            if (ByteCount & 0x1)
            {
                DebugPrint("ERROR - Byte count not aligned");
                return ApiInvalidSize;
            }
            break;

        case BitSize32:
            if (offset & 0x3)
            {
                DebugPrint("ERROR - Local address not aligned");
                return ApiInvalidAddress;
            }

            if (ByteCount & 0x3)
            {
                DebugPrint("ERROR - Byte count not aligned");
                return ApiInvalidSize;
            }
            break;

        default:
            DebugPrint("ERROR - Invalid access type");
            return ApiInvalidAccessType;
    }

    // Get offset of remap register
    PlxChipGetRemapOffset(
        pdx,
        BarIndex,
        &Offset_RegRemap
        );

    if (Offset_RegRemap == (U16)-1)
    {
        return ApiInvalidIopSpace;
    }

    // Only memory spaces are supported by this function
    if (pdx->PciBar[BarIndex].Properties.bIoSpace)
    {
        DebugPrint("ERROR - I/O spaces not supported by this function");
        return ApiInvalidIopSpace;
    }

    // Get kernel virtual address for the space
    SpaceVa = (PLX_UINT_PTR)pdx->PciBar[BarIndex].pVa;

    if (SpaceVa == 0)
    {
        DebugPrint(
            "ERROR - Invalid kernel VA (0x%08lx) for PCI BAR\n",
            SpaceVa
            );

        return ApiInvalidAddress;
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
            DebugPrint("ERROR - requested area exceeds space range");
            return ApiInvalidSize;
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
            // Copy block to user buffer
            switch (AccessType)
            {
                case BitSize8:
                    DEV_MEM_TO_USER_8(
                        pBuffer,
                        (SpaceVa + SpaceOffset),
                        BytesToTransfer
                        );
                    break;

                case BitSize16:
                    DEV_MEM_TO_USER_16(
                        pBuffer,
                        (SpaceVa + SpaceOffset),
                        BytesToTransfer
                        );
                    break;

                case BitSize32:
                    DEV_MEM_TO_USER_32(
                        pBuffer,
                        (SpaceVa + SpaceOffset),
                        BytesToTransfer
                        );
                    break;

                case BitSize64:
                    // 64-bit not implemented yet
                    break;
            }
        }
        else
        {
            // Copy user buffer to device memory
            switch (AccessType)
            {
                case BitSize8:
                    USER_TO_DEV_MEM_8(
                        (SpaceVa + SpaceOffset),
                        pBuffer,
                        BytesToTransfer
                        );
                    break;

                case BitSize16:
                    USER_TO_DEV_MEM_16(
                        (SpaceVa + SpaceOffset),
                        pBuffer,
                        BytesToTransfer
                        );
                    break;

                case BitSize32:
                    USER_TO_DEV_MEM_32(
                        (SpaceVa + SpaceOffset),
                        pBuffer,
                        BytesToTransfer
                        );
                    break;

                case BitSize64:
                    // 64-bit not implemented yet
                    break;
            }
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

    return ApiSuccess;
}




/*******************************************************************************
 *
 * Function   :  PlxPciVpdRead
 *
 * Description:  Read the Vital Product Data
 *
 ******************************************************************************/
PLX_STATUS
PlxPciVpdRead(
    DEVICE_EXTENSION *pdx,
    U16               offset,
    U32              *pValue
    )
{
    S16 VpdRetries;
    S16 VpdPollCount;
    U16 Offset_VPD;
    U32 RegValue;


    // Check for unaligned offset
    if (offset & 0x3)
    {
        *pValue = (U32)-1;
        return ApiInvalidOffset;
    }

    // Get the VPD offset
    Offset_VPD =
        PlxGetExtendedCapabilityOffset(
            pdx,
            CAP_ID_VPD
            );

    if (Offset_VPD == 0)
    {
        return ApiUnsupportedFunction;
    }

     /**********************************************
      * The EEDO Input (bit 31) must be disabled
      * for some chips when VPD access is used.
      * Since it is a reserved bit in older chips,
      * there is no harm in clearing it for all.
      *********************************************/
    RegValue =
        PLX_9000_REG_READ(
            pdx,
            REG_EEPROM_CTRL
            );

    PLX_9000_REG_WRITE(
        pdx,
        REG_EEPROM_CTRL,
        RegValue & ~(1 << 31)
        );

    // Prepare VPD command
    RegValue = ((U32)offset << 16) | 0x3;

    VpdRetries = VPD_COMMAND_MAX_RETRIES;
    do
    {
        /**************************************
        *  This loop will continue until the
        *  VPD reports success or until we reach
        *  the maximum number of retries
        **************************************/

        // Send VPD Command
        PLX_PCI_REG_WRITE(
            pdx,
            Offset_VPD,
            RegValue
            );

        // Poll until VPD operation has completed
        VpdPollCount = VPD_STATUS_MAX_POLL;
        do
        {
            // Delay for a bit for VPD operation
            Plx_sleep(VPD_STATUS_POLL_DELAY);

            // Get VPD Status
            PLX_PCI_REG_READ(
                pdx,
                Offset_VPD,
                &RegValue
                );

            // Check for command completion
            if (RegValue & (1 << 31))
            {
                // VPD read completed successfully

                // Get the VPD Data result
                PLX_PCI_REG_READ(
                    pdx,
                    Offset_VPD + sizeof(U32),
                    pValue
                    );

                return ApiSuccess;
            }
        }
        while (VpdPollCount--);
    }
    while (VpdRetries--);

    // At this point, VPD access failed
    DebugPrint("ERROR - Timeout waiting for VPD read to complete\n");

    *pValue = (U32)-1;

    return ApiWaitTimeout;
}




/*******************************************************************************
 *
 * Function   :  PlxPciVpdWrite
 *
 * Description:  Write to the Vital Product Data
 *
 ******************************************************************************/
PLX_STATUS
PlxPciVpdWrite(
    DEVICE_EXTENSION *pdx,
    U16               offset,
    U32               VpdData
    )
{
    S16 VpdRetries;
    S16 VpdPollCount;
    U16 Offset_VPD;
    U32 RegValue;


    // Check for unaligned offset
    if (offset & 0x3)
        return ApiInvalidOffset;

    // Get the VPD offset
    Offset_VPD =
        PlxGetExtendedCapabilityOffset(
            pdx,
            CAP_ID_VPD
            );

    if (Offset_VPD == 0)
    {
        return ApiUnsupportedFunction;
    }

     /**********************************************
      * The EEDO Input (bit 31) must be disabled
      * for some chips when VPD access is used.
      * Since it is a reserved bit in older chips,
      * there is no harm in clearing it for all.
      *********************************************/
    RegValue =
        PLX_9000_REG_READ(
            pdx,
            REG_EEPROM_CTRL
            );

    PLX_9000_REG_WRITE(
        pdx,
        REG_EEPROM_CTRL,
        RegValue & ~(1 << 31)
        );

    // Put write value into VPD Data register
    PLX_PCI_REG_WRITE(
        pdx,
        Offset_VPD + sizeof(U32),
        VpdData
        );

    // Prepare VPD command
    RegValue = (1 << 31) | ((U32)offset << 16) | 0x3;

    VpdRetries = VPD_COMMAND_MAX_RETRIES;
    do
    {
        /**************************************
        *  This loop will continue until the
        *  VPD reports success or until we reach
        *  the maximum number of retries
        **************************************/

        // Send VPD command
        PLX_PCI_REG_WRITE(
            pdx,
            Offset_VPD,
            RegValue
            );

        // Poll until VPD operation has completed
        VpdPollCount = VPD_STATUS_MAX_POLL;
        do
        {
            // Delay for a bit for VPD operation
            Plx_sleep(VPD_STATUS_POLL_DELAY);

            // Get VPD Status
            PLX_PCI_REG_READ(
                pdx,
                Offset_VPD,
                &RegValue
                );

            // Check for command completion
            if ((RegValue & (1 << 31)) == 0)
            {
                // VPD write completed successfully
                return ApiSuccess;
            }
        }
        while (VpdPollCount--);
    }
    while (VpdRetries--);

    // At this point, VPD access failed
    DebugPrint("ERROR - Timeout waiting for VPD write to complete\n");

    return ApiWaitTimeout;
}
