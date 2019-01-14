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
 *      PciFunc.c
 *
 * Description:
 *
 *      This file contains the PCI support functions
 *
 * Revision History:
 *
 *      12-01-07 : PLX SDK v5.20
 *
 ******************************************************************************/


#include "PciFunc.h"
#include "PlugPlay.h"
#include "SuppFunc.h"




/***********************************************
 *               Globals
 **********************************************/
// Global ECAM 64-bit address from ACPI table (0=Not Probed, -1=Not Available)
static U32 Gbl_Acpi_Addr_ECAM[2] = {ACPI_PCIE_NOT_PROBED, ACPI_PCIE_NOT_PROBED};




/******************************************************************************
 *
 * Function   :  PciRegisterRead_UsingPnP
 *
 * Description:  Read a PCI register of a device using the PnP Bus Manager
 *
 ******************************************************************************/
PLX_STATUS
PciRegisterRead_UsingPnP(
    DEVICE_OBJECT *fdo,
    U16            offset,
    VOID          *pBuffer,
    U8             size
    )
{
    PIRP               pIrp;
    KEVENT             event;
    NTSTATUS           status;
    PIO_STACK_LOCATION stack;


    pIrp =
        IoAllocateIrp(
            fdo->StackSize,
            FALSE
            );

    if (pIrp == NULL)
    {
        DebugPrintf(("ERROR - Unable to allocate IRP\n"));
        return ApiInsufficientResources;
    }

    // Initialize kernel event
    KeInitializeEvent(
        &event,
        NotificationEvent,
        FALSE
        );

    stack =
        IoGetNextIrpStackLocation(
            pIrp
            );

    // Fill the IRP
    pIrp->IoStatus.Status                        = STATUS_NOT_SUPPORTED;
    stack->MajorFunction                         = IRP_MJ_PNP;
    stack->MinorFunction                         = IRP_MN_READ_CONFIG;
    stack->Parameters.ReadWriteConfig.WhichSpace = PCI_WHICHSPACE_CONFIG;
    stack->Parameters.ReadWriteConfig.Buffer     = pBuffer;
    stack->Parameters.ReadWriteConfig.Offset     = offset;
    stack->Parameters.ReadWriteConfig.Length     = size;

    IoSetCompletionRoutine(
        pIrp,
        (PIO_COMPLETION_ROUTINE)OnRequestComplete,
        (PVOID)&event,
        TRUE,
        TRUE,
        TRUE
        );

    // Send the packet
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

        status = pIrp->IoStatus.Status;
    }

    // Release the IRP
    IoFreeIrp(
        pIrp
        );

    if (status != STATUS_SUCCESS)
        return ApiConfigAccessFailed;

    return ApiSuccess;
}




/******************************************************************************
 *
 * Function   :  PciRegisterWrite_UsingPnP
 *
 * Description:  Writes to a PCI register of a device using the PnP Bus Manager
 *
 ******************************************************************************/
PLX_STATUS
PciRegisterWrite_UsingPnP(
    PDEVICE_OBJECT  fdo,
    U16             offset,
    U32             value
    )
{
    PIRP               pIrp;
    KEVENT             event;
    NTSTATUS           status;
    PIO_STACK_LOCATION stack;


    pIrp =
        IoAllocateIrp(
            fdo->StackSize,
            FALSE
            );

    if (pIrp == NULL)
    {
        DebugPrintf(("ERROR - Unable to allocate IRP\n"));
        return ApiInsufficientResources;
    }

    // Initialize kernel event
    KeInitializeEvent(
        &event,
        NotificationEvent,
        FALSE
        );

    stack =
        IoGetNextIrpStackLocation(
            pIrp
            );

    // Fill the IRP
    pIrp->IoStatus.Status                        = STATUS_NOT_SUPPORTED;
    stack->MajorFunction                         = IRP_MJ_PNP;
    stack->MinorFunction                         = IRP_MN_WRITE_CONFIG;
    stack->Parameters.ReadWriteConfig.WhichSpace = PCI_WHICHSPACE_CONFIG;
    stack->Parameters.ReadWriteConfig.Buffer     = &value;
    stack->Parameters.ReadWriteConfig.Offset     = offset;
    stack->Parameters.ReadWriteConfig.Length     = sizeof(U32);

    IoSetCompletionRoutine(
        pIrp,
        (PIO_COMPLETION_ROUTINE)OnRequestComplete,
        (PVOID)&event,
        TRUE,
        TRUE,
        TRUE
        );

    // Send the packet
    status =
        IoCallDriver(
            ((DEVICE_EXTENSION *)fdo->DeviceExtension)->pLowerDeviceObject,
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

        status = pIrp->IoStatus.Status;
    }

    IoFreeIrp(
        pIrp
        );

    if (status != STATUS_SUCCESS)
        return ApiConfigAccessFailed;

    return ApiSuccess;
}




/******************************************************************************
 *
 * Function   :  PlxPciRegisterRead_Unsupported
 *
 * Description:  Reads a PCI register by bypassing the OS services
 *
 ******************************************************************************/
PLX_STATUS
PlxPciRegisterRead_Unsupported(
    U8   bus,
    U8   slot,
    U8   function,
    U16  offset,
    U32 *pValue
    )
{
    U32   RegSave;
    KIRQL IrqlSave;


    // For PCIe extended register, use Enhanced PCIe Mechanism
    if (offset >= 0x100)
    {
        return PlxPciExpressRegRead(
                   bus,
                   slot,
                   function,
                   offset,
                   pValue
                   );
    }

    /***************************************************************
     * Access of a PCI register involves using I/O addresses 0xcf8
     * and 0xcfc. These addresses must be used together and no other
     * process must interrupt.
     *
     * Note:  On Multi-Processor systems, the I/O ports may still be
     *        accessed by another CPU.  Raising the IRQL does not
     *        prevent other CPUs from accessing the ports since
     *        they are not a resource owned by this driver.
     **************************************************************/

    // Make sure slot is only 5-bits
    slot &= 0x1F;

    // Make sure function is only 3-bits
    function &= 0x7;

    // Raise the IRQL to prevent context switches during access
    KeRaiseIrql(
        DISPATCH_LEVEL,
        &IrqlSave
        );

    // Save the content of the command register
    RegSave =
        IO_PORT_READ_32(
            (ULONG*)0xcf8
            );

    // Configure the command register to access the desired location
    IO_PORT_WRITE_32(
        (ULONG*)0xcf8,
        (1 << 31) | (bus << 16) | (slot << 11) | (function << 8) | offset
        );

    // Read the register
    *pValue =
        IO_PORT_READ_32(
            (ULONG*)0xcfc
            );

    // Restore the command register
    IO_PORT_WRITE_32(
        (ULONG*)0xcf8,
        RegSave
        );

    // Restore IRQL
    KeLowerIrql(
        IrqlSave
        );

    return ApiSuccess;
}




/******************************************************************************
 *
 * Function   :  PlxPciRegisterWrite_Unsupported
 *
 * Description:  Writes to a PCI register by bypassing the OS services
 *
 ******************************************************************************/
PLX_STATUS
PlxPciRegisterWrite_Unsupported(
    U8  bus,
    U8  slot,
    U8  function,
    U16 offset,
    U32 value
    )
{
    U32   RegSave;
    KIRQL IrqlSave;


    // For PCIe extended register, use Enhanced PCIe Mechanism
    if (offset >= 0x100)
    {
        return PlxPciExpressRegWrite(
                   bus,
                   slot,
                   function,
                   offset,
                   value
                   );
    }

    /***************************************************************
     * Access of a PCI register involves using I/O addresses 0xcf8
     * and 0xcfc. These addresses must be used together and no other
     * process must interrupt.
     *
     * Note:  On Multi-Processor systems, the I/O ports may still be
     *        accessed by another CPU.  Raising the IRQL does not
     *        prevent other CPUs from accessing the ports since
     *        they are not a resource owned by this driver.
     **************************************************************/

    // Make sure slot is only 5-bits
    slot &= 0x1F;

    // Make sure function is only 3-bits
    function &= 0x7;

    // Raise the IRQL to prevent context switches during access
    KeRaiseIrql(
        DISPATCH_LEVEL,
        &IrqlSave
        );

    // Save the content of the command register
    RegSave =
        IO_PORT_READ_32(
            (ULONG*)0xcf8
            );

    // Configure the command register to access the desired location
    IO_PORT_WRITE_32(
        (ULONG*)0xcf8,
        (1 << 31) | (bus << 16) | (slot << 11) | (function << 8) | offset
        );

    // Write the register
    IO_PORT_WRITE_32(
        (ULONG*)0xcfc,
        value
        );

    // Restore the command register
    IO_PORT_WRITE_32(
        (ULONG*)0xcf8,
        RegSave
        );

    // Restore IRQL
    KeLowerIrql(
        IrqlSave
        );

    return ApiSuccess;
}




/******************************************************************************
 *
 * Function   :  PlxPciExpressRegRead
 *
 * Description:  Reads a PCI Express register using the enhanced configuration mechanism
 *
 *****************************************************************************/
PLX_STATUS
PlxPciExpressRegRead(
    U8   bus,
    U8   slot,
    U8   function,
    U16  offset,
    U32 *pValue
    )
{
    U32 address;


    // Offset must on a 4-byte boundary
    if (offset & 0x3)
    {
        return ApiInvalidOffset;
    }

    // Check if PCIe ECAM was probed for
    if (Gbl_Acpi_Addr_ECAM[0] == ACPI_PCIE_NOT_PROBED)
        PlxProbeForEcamBase();

    // Check if Enhanced mechanism available through ACPI
    if (Gbl_Acpi_Addr_ECAM[0] == ACPI_PCIE_NOT_AVAILABLE)
    {
        *pValue = (U32)-1;
        return ApiUnsupportedFunction;
    }

    // Setup base address for register access
    address =
        Gbl_Acpi_Addr_ECAM[0] |
        (bus      << 20)      |
        (slot     << 15)      |
        (function << 12)      |
        (offset   <<  0);

    // Read the register
    *pValue = PHYS_MEM_READ_32(address);

    return ApiSuccess;
}




/******************************************************************************
 *
 * Function   :  PlxPciExpressRegWrite
 *
 * Description:  Writes a PCI Express register using the enhanced configuration mechanism
 *
 *****************************************************************************/
PLX_STATUS
PlxPciExpressRegWrite(
    U8  bus,
    U8  slot,
    U8  function,
    U16 offset,
    U32 value
    )
{
    U32 address;


    // Offset must on a 4-byte boundary
    if (offset & 0x3)
    {
        return ApiInvalidOffset;
    }

    // Check if PCIe ECAM was probed for
    if (Gbl_Acpi_Addr_ECAM[0] == ACPI_PCIE_NOT_PROBED)
        PlxProbeForEcamBase();

    // Check if Enhanced mechanism available through ACPI
    if (Gbl_Acpi_Addr_ECAM[0] == ACPI_PCIE_NOT_AVAILABLE)
    {
        return ApiUnsupportedFunction;
    }

    // Setup base address for register access
    address =
        Gbl_Acpi_Addr_ECAM[0] |
        (bus      << 20)      |
        (slot     << 15)      |
        (function << 12)      |
        (offset   <<  0);

    // Write the register
    PHYS_MEM_WRITE_32(address, value);

    return ApiSuccess;
}




/******************************************************************************
 *
 * Function   :  PlxProbeForEcamBase
 *
 * Description:  Probes for Enhanced Configuration Access Mechanism base
 *               address through ACPI
 *
 *****************************************************************************/
VOID
PlxProbeForEcamBase(
    VOID
    )
{
    U8             Str_ID[9];
    U32            Entry;
    U32            address;
    U32            value;
    U32            NumEntries;
    U32            Acpi_Addr_RSDP;
    U32            Acpi_Addr_RSDT;
    BOOLEAN        bFound;
    ACPI_RSDT_v1_0 Acpi_Rsdt;


    // Added for compiler warning
    Acpi_Addr_RSDP = 0;

    // Do not probe again if previously did
    if (Gbl_Acpi_Addr_ECAM[0] != ACPI_PCIE_NOT_PROBED)
       return;

    // Default to ACPI and/or ECAM not detected
    Gbl_Acpi_Addr_ECAM[0] = ACPI_PCIE_NOT_AVAILABLE;

    // Mark end of string
    Str_ID[8] = '\0';

    // Setup starting address on 16-byte boundary
    address = BIOS_MEM_START;
    address = (address + 0xF) & ~0xF;

    bFound = FALSE;

    // Scan system ROM for ACPI RSDP pointer
    do
    {
        // Read 8-bytes
        *(U32*)Str_ID       = PHYS_MEM_READ_32(address);
        *(U32*)(Str_ID + 4) = PHYS_MEM_READ_32(address + 4);

        // Check for header signature
        if (memcmp(
                "RSD PTR ",
                Str_ID,
                8       // 8 bytes
                ) == 0)
        {
            // Store ACPI RSDP table address
            Acpi_Addr_RSDP = address;

            bFound = TRUE;
        }
        else
        {
            // Increment to next 16-byte boundary
            address += 16;
        }
    }
    while (!bFound && (address <= BIOS_MEM_END));

    if (!bFound)
    {
        DebugPrintf(("ACPI Probe - ACPI not detected\n"));
        return;
    }

    DebugPrintf((
        "ACPI Probe - 'RSD PTR ' found at 0x%08X\n",
        Acpi_Addr_RSDP
        ));

    // Store RSDT address
    Acpi_Addr_RSDT = PHYS_MEM_READ_32(Acpi_Addr_RSDP + 16);

    // Get RSDT size
    Acpi_Rsdt.Length = PHYS_MEM_READ_32(Acpi_Addr_RSDT + 4);

    if (Acpi_Rsdt.Length == 0)
    {
        DebugPrintf(("ACPI Probe - Unable to read RSDT table length\n"));
        return;
    }

    // Calculate number of entries
    NumEntries = (Acpi_Rsdt.Length - sizeof(ACPI_RSDT_v1_0)) / sizeof(U32);

    if (NumEntries > 20)
    {
        DebugPrintf(("ACPI Probe - Unable to determine number of entries in RSDT table\n"));
        return;
    }

    DebugPrintf((
        "ACPI Probe - RSDT table at 0x%08X has %d entries\n",
        Acpi_Addr_RSDT,
        NumEntries
        ));

    // Get address of first entry
    Entry = Acpi_Addr_RSDT + sizeof(ACPI_RSDT_v1_0);

    bFound = FALSE;

    // Parse entry pointers for MCFG table
    while ((bFound == FALSE) && (NumEntries != 0))
    {
        // Get address of entry
        address = PHYS_MEM_READ_32(Entry);

        // Get table signature
        value = PHYS_MEM_READ_32(address);

        DebugPrintf((
            "ACPI Probe - Located '%c%c%c%c' table at 0x%08X\n",
            (char)(value >>  0),
            (char)(value >>  8),
            (char)(value >> 16),
            (char)(value >> 24),
            address
            ));

        // Check if MCFG table
        if (memcmp(
                "MCFG",
                &value,
                sizeof(U32)
                ) == 0)
        {
            // Get 64-bit base address of Enhanced Config Access Mechanism
            Gbl_Acpi_Addr_ECAM[0] = PHYS_MEM_READ_32(address + 44);
            Gbl_Acpi_Addr_ECAM[1] = PHYS_MEM_READ_32(address + 48);

            bFound = TRUE;
        }
        else
        {
            // Get address of next entry
            Entry += sizeof(U32);

            // Decrement count
            NumEntries--;
        }
    }

    if (bFound)
    {
        DebugPrintf((
            "ACPI Probe - PCIe ECAM is located at 0x%08X_%08X\n",
            Gbl_Acpi_Addr_ECAM[1], Gbl_Acpi_Addr_ECAM[0]
            ));
    }
    else
    {
        DebugPrintf((
            "ACPI Probe - MCFG entry not found (PCIe ECAM not supported)\n"
            ));
    }
}




/******************************************************************************
 *
 * Function   :  PlxPhysicalMemRead
 *
 * Description:  Maps a memory location and performs a read from it
 *
 *****************************************************************************/
U32
PlxPhysicalMemRead(
    U32 address,
    U8  size
    )
{
    U32               value;
    VOID             *KernelVa;
    PHYSICAL_ADDRESS  PhysAddr;


    PhysAddr.QuadPart = address;

    // Map address into kernel space
    KernelVa =
        MmMapIoSpace(
            PhysAddr,
            sizeof(U32),
            MmNonCached
            );

    if (KernelVa == NULL)
    {
        DebugPrintf(("ERROR: Unable to map address into kernel space\n"));
        return -1;
    }

    // Read memory
    switch (size)
    {
        case sizeof(U8):
            value = *(U8*)KernelVa;
            break;

        case sizeof(U16):
            value = *(U16*)KernelVa;
            break;

        case sizeof(U32):
            value = *(U32*)KernelVa;
            break;

        default:
            value = 0;
    }

    // Release the mapping
    MmUnmapIoSpace(
        KernelVa,
        sizeof(U32)
        );

    return value;
}





/******************************************************************************
 *
 * Function   :  PlxPhysicalMemWrite
 *
 * Description:  Maps a memory location and performs a write to it
 *
 *****************************************************************************/
U32
PlxPhysicalMemWrite(
    U32 address,
    U32 value,
    U8  size
    )
{
    VOID             *KernelVa;
    PHYSICAL_ADDRESS  PhysAddr;


    PhysAddr.QuadPart = address;

    // Map address into kernel space
    KernelVa =
        MmMapIoSpace(
            PhysAddr,
            sizeof(U32),
            MmNonCached
            );

    if (KernelVa == NULL)
    {
        DebugPrintf(("ERROR: Unable to map address into kernel space\n"));
        return -1;
    }

    // Write memory
    switch (size)
    {
        case sizeof(U8):
            *(U8*)KernelVa = (U8)value;
            break;

        case sizeof(U16):
            *(U16*)KernelVa = (U16)value;
            break;

        case sizeof(U32):
            *(U32*)KernelVa = (U32)value;
            break;
    }

    // Release the mapping
    MmUnmapIoSpace(
        KernelVa,
        sizeof(U32)
        );

    return 0;
}




/******************************************************************************
 *
 * Function   :  GetBusSlotNumber
 *
 * Description:  Get the bus and slot number of a supported device
 *
 ******************************************************************************/
NTSTATUS
GetBusSlotNumber(
    DEVICE_OBJECT    *pdo,
    DEVICE_EXTENSION *pdx,
    U32               CompareValue
    )
{
    U8       slot;
    U32      ResultLength;
    U32      PropertyBuffer;
    U32      RegValue;
    U32      RegisterSave;
    NTSTATUS status;


    ResultLength = 0;

    // Get the bus number
    status =
        IoGetDeviceProperty(
             pdo,
             DevicePropertyBusNumber,
             sizeof(U32),
             &PropertyBuffer,
             &ResultLength
             );

    if ( !NT_SUCCESS(status) )
    {
        DebugPrintf((
            "ERROR - IoGetDeviceProperty() unable to get bus number, code = %Xh\n",
            status
            ));

        return status;
    }

    // Verify that the function did write back a U32
    if (ResultLength != sizeof(U32))
    {
        DebugPrintf((
            "ERROR - IoGetDeviceProperty() invalid ResultLength (%d)\n",
            ResultLength
            ));

        return STATUS_UNSUCCESSFUL;
    }

    // Store the Bus number
    pdx->Key.bus = (U8)PropertyBuffer;


    /**************************************************************************
     * Now attempt to get the slot number
     *
     * Note: The IoGetDeviceProperty() function is not implemented correctly in
     *       Windows 98.  The code has been left here in case this is fixed in a
     *       future release.  If the IoGetDeviceProperty() function is unsuccessful,
     *       a workaround is provided to retrieve the correct slot number.
     **************************************************************************/

    ResultLength = 0;

    status =
        IoGetDeviceProperty(
            pdo,
            DevicePropertyAddress,
            sizeof(U32),
            &PropertyBuffer,
            &ResultLength
            );

    if ( NT_SUCCESS(status) )
    {
        if (ResultLength != sizeof(U32))
        {
            DebugPrintf((
                "ERROR - IoGetDeviceProperty() invalid ResultLength (%d)\n",
                ResultLength
                ));

            return STATUS_UNSUCCESSFUL;
        }

        pdx->Key.slot     = (U8)(PropertyBuffer >> 16);
        pdx->Key.function = (U8)PropertyBuffer;

        DebugPrintf((
            "Device information - PCI bus %02x, slot %02x, function 0\n",
            pdx->Key.bus,
            (U8)(PropertyBuffer >> 16)
            ));

        return STATUS_SUCCESS;
    }


    /****************************************************************
    * We were unable to get the slot number, so another method is
    * required.  Our workaround is to scan the PCI bus and find the
    * device whose BAR0 matches the current device.
    ****************************************************************/

    DebugPrintf(("WARNING - IoGetDeviceProperty() unable to get slot number, code = "));

    switch (status)
    {
        case STATUS_BUFFER_TOO_SMALL:
            DebugPrintf_NoInfo(("STATUS_BUFFER_TOO_SMALL (req=%d bytes)\n", ResultLength));
            break;

        case STATUS_NOT_IMPLEMENTED:
            DebugPrintf_NoInfo(("STATUS_NOT_IMPLEMENTED\n"));
            break;

        case STATUS_NOT_FOUND:
            DebugPrintf_NoInfo(("STATUS_NOT_FOUND\n"));
            break;

        default:
            DebugPrintf_NoInfo(("%08xh\n", status));
            break;
    }

    DebugPrintf(("NOTE: Implementing workaround to get Slot number\n"));

    // Scan the PCI bus to find our device
    for (slot = 0; slot < MAX_PCI_DEV; slot++)
    {
        // Read the BAR register
        PlxPciRegisterRead(
            pdx->Key.bus,
            slot,
            0,
            0x10,        // PCI BAR 0
            &RegValue
            );

        // Compare with our device
        if (RegValue == CompareValue)
        {
            pdx->Key.slot = slot;

            DebugPrintf((
                "Workaround successful, device information - PCI bus %02x, slot %02x, function 0\n",
                pdx->Key.bus,
                slot
                ));

            return STATUS_SUCCESS;
        }
    }

    DebugPrintf(("ERROR - Unable to implement workaround to get Slot number\n"));

    pdx->Key.slot = -1;

    return STATUS_UNSUCCESSFUL;
}
