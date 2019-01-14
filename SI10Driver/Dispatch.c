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
 *      Dispatch.c
 *
 * Description:
 *
 *      This file routes incoming I/O Request packets
 *
 * Revision History:
 *
 *      08-01-08 : PLX SDK v6.00
 *
 ******************************************************************************/


#include "ApiFunc.h"
#include "Dispatch.h"
#include "GlobalVars.h"
#include "PciFunc.h"
#include "PlxChipApi.h"
#include ".\Include\PlxIoctl.h"
#include "SuppFunc.h"
#include "DebugPrint.h"
#include "Si10Ioctl.h"
#include "..\Si6Driver\sys\Si6IOctl.h"
#include "PlxChipFn.h"
#include "..\USLDriver\sys\USLIoctl.h"


/******************************************************************************
 *
 * Function   :  Dispatch_Create
 *
 * Description:  Handle IRP_MJ_CREATE, which allows applications to open handles
 *               to our device
 *
 ******************************************************************************/
NTSTATUS
Dispatch_Create(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    )
{

	DEVICE_EXTENSION *pdx = (DEVICE_EXTENSION *) fdo->DeviceExtension;
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(pIrp);

    DebugPrint(
        "Received message (IRP=0x%p) ===> IRP_MJ_CREATE\n",
        pIrp
        );

    return PlxCompleteIrpWithInformation(
               pIrp,
               STATUS_SUCCESS,
               0
               );
}




/******************************************************************************
 *
 * Function   :  Dispatch_Cleanup
 *
 * Description:  Handle the IRP_MJ_CLEANUP IRP
 *
 ******************************************************************************/
NTSTATUS
Dispatch_Cleanup(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    )
{
    PIO_STACK_LOCATION pStack;


    DebugPrint("Received message (IRP) ===> IRP_MJ_CLEANUP\n");

    pStack =
        IoGetCurrentIrpStackLocation(
            pIrp
            );

    // Release any pending notifications owned by proccess
    PlxNotificationCancel(
        fdo->DeviceExtension,
        NULL,
        pStack->FileObject
        );

    // Close DMA channels owned by the process
    PlxDmaChannelCleanup(
        fdo->DeviceExtension,
        pStack->FileObject
        );

    // Unmap any mappings to PCI BAR spaces owned by process
    PlxPciBarSpaceUnmapAll_ByOwner(
        fdo->DeviceExtension,
        pStack->FileObject
        );

    // Unmap any mappings to the common buffer owned by process
    PlxPciPhysicalMemoryUnmapAll_ByOwner(
        fdo->DeviceExtension,
        pGbl_CommonBuffer,
        pStack->FileObject
        );

    // Unmap and deallocate any physical memory owned by process
    PlxPciPhysicalMemoryFreeAll_ByOwner(
        fdo->DeviceExtension,
        pStack->FileObject
        );

    return PlxCompleteIrpWithInformation(
               pIrp,
               STATUS_SUCCESS,
               0
               );
}




/******************************************************************************
 *
 * Function   :  Dispatch_Close
 *
 * Description:  Handle IRP_MJ_CLOSE, which allows applications to close handles
 *               to our device
 *
 ******************************************************************************/
NTSTATUS
Dispatch_Close(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    )
{
	DebugPrint(
        "Received message (IRP=0x) ===> IRP_MJ_CLOSE\n",    pIrp
       );

	WriteDWord(fdo->DeviceExtension, 0x3C4, 0x11);

	FreeUserStruct(fdo->DeviceExtension);
	FreeTimeSlot(fdo->DeviceExtension);
	FreeUtUserStruct( fdo->DeviceExtension);
	DeRegisterEvent( fdo->DeviceExtension);

	WriteDWord(fdo->DeviceExtension, 0x3C4, 0x11);

    return PlxCompleteIrpWithInformation(
               pIrp,
               STATUS_SUCCESS,
               0
               );
}




/******************************************************************************
 *
 * Function   :  Dispatch_SystemControl
 *
 * Description:  The dispatch routine for WMI IRPs.  It does nothing except
 *               forward the IRP to the next device in the stack.
 *
 * Note       :  This routine is required or DriverVerifier will bug check
 *
 ******************************************************************************/
NTSTATUS
Dispatch_SystemControl(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    )
{


    DebugPrint("Forwarded IRP to next lower driver\n");

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
 * Function   :  Dispatch_IoControl
 *
 * Description:  Processes the IOCTL messages sent to this device
 *
 ******************************************************************************/
NTSTATUS
Dispatch_IoControl(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    )
{
    VOID               *pOwner;
    PLX_PARAMS         *pIoBuffer;
    DEVICE_EXTENSION   *pdx;
    PIO_STACK_LOCATION  pStack;
	ULONG				ControlCode;
	ULONG				InputLength;
	ULONG				OutputLength;
	int					ii;
	char *pData;
	char Data;
	ULONG Value;
	int bit,nSize;
	NTSTATUS status;


    pdx = fdo->DeviceExtension;


    pStack =
        IoGetCurrentIrpStackLocation(
            pIrp
            );

	
	if ( (pStack->Parameters.DeviceIoControl.IoControlCode != IOCTL_SI10_SHARE_USER_STRUCT) &&
		(pStack->Parameters.DeviceIoControl.IoControlCode != IOCTL_SI10_LOAD_FPGA_CODE) &&
		(pStack->Parameters.DeviceIoControl.IoControlCode != IOCTL_SI10_ENABLE_INTERRUPT_CALLBACK) )

	{
		// Track the owner
		pOwner = pStack->FileObject;
		pIoBuffer                  = pIrp->AssociatedIrp.SystemBuffer;
		pIrp->IoStatus.Information = sizeof(PLX_PARAMS);
		
	}


	if(pStack->Parameters.DeviceIoControl.IoControlCode != PLX_IOCTL_REGISTER_WRITE) {
		ControlCode = pStack->Parameters.DeviceIoControl.IoControlCode;
		InputLength = pStack->Parameters.DeviceIoControl.InputBufferLength;
		OutputLength = pStack->Parameters.DeviceIoControl.OutputBufferLength;

		DebugPrint("DeviceIoControl: Control code %x InputLength %d OutputLength %d",ControlCode, InputLength, OutputLength);
	}

    // Check for messages that require D0 power state
    if (pdx->PowerState > MIN_WORKING_POWER_STATE)
    {
        switch (pStack->Parameters.DeviceIoControl.IoControlCode)
        {
            case PLX_IOCTL_CHIP_TYPE_GET:
            case PLX_IOCTL_REGISTER_READ:
            case PLX_IOCTL_REGISTER_WRITE:
            case PLX_IOCTL_MAPPED_REGISTER_READ:
            case PLX_IOCTL_MAPPED_REGISTER_WRITE:
            case PLX_IOCTL_EEPROM_PRESENT:
            case PLX_IOCTL_EEPROM_PROBE:
            case PLX_IOCTL_EEPROM_READ_BY_OFFSET:
            case PLX_IOCTL_EEPROM_WRITE_BY_OFFSET:
            case PLX_IOCTL_EEPROM_READ_BY_OFFSET_16:
            case PLX_IOCTL_EEPROM_WRITE_BY_OFFSET_16:
            case PLX_IOCTL_INTR_ENABLE:
            case PLX_IOCTL_INTR_DISABLE:
            case PLX_IOCTL_PCI_BAR_SPACE_READ:
            case PLX_IOCTL_PCI_BAR_SPACE_WRITE:
            case PLX_IOCTL_DMA_CONTROL:
            case PLX_IOCTL_DMA_STATUS:
            case PLX_IOCTL_DMA_CHANNEL_OPEN:
            case PLX_IOCTL_DMA_TRANSFER_BLOCK:
            case PLX_IOCTL_DMA_TRANSFER_USER_BUFFER:
            case PLX_IOCTL_DMA_CHANNEL_CLOSE:
		        DebugPrint("ERROR - Device is in low power state, cannot continue\n");
                pIoBuffer->ReturnCode = ApiPowerDown;
                goto _Exit_Dispatch_IoControl;
        }
    }

    // Handle the PLX specific message
    switch (pStack->Parameters.DeviceIoControl.IoControlCode)
    {
        /******************************************
         * Driver Query Functions
         *****************************************/
        case PLX_IOCTL_PCI_DEVICE_FIND:
            DebugPrint("PLX_IOCTL_PCI_DEVICE_FIND\n");

            pIoBuffer->ReturnCode =
                PlxDeviceFind(
                    pdx,
                    &(pIoBuffer->Key),
                    (U8*)&(pIoBuffer->value[0])
                    );
            break;

        case PLX_IOCTL_DRIVER_VERSION:
            DebugPrint("PLX_IOCTL_DRIVER_VERSION\n");

            pIoBuffer->value[0] =
                (SI10_SDK_VERSION_MAJOR    << 16) |
                (SI10_SDK_VERSION_MINOR    <<  8) |
                (SI10_SDK_VERSION_REVISION <<  0);
            break;
 
        case PLX_IOCTL_CHIP_TYPE_GET:
            DebugPrint("PLX_IOCTL_CHIP_TYPE_GET\n");

            pIoBuffer->ReturnCode =
                PlxChipTypeGet(
                    pdx,
                    &(pIoBuffer->Key),
                    (U32*)&(pIoBuffer->value[0]),
                    (U8*)&(pIoBuffer->value[1])
                    );
            break;

        case PLX_IOCTL_CHIP_TYPE_SET:
            DebugPrint("PLX_IOCTL_CHIP_TYPE_SET\n");

            pIoBuffer->ReturnCode =
                PlxChipTypeSet(
                    pdx,
                    &(pIoBuffer->Key),
                    (U32)pIoBuffer->value[0],
                    (U8)pIoBuffer->value[1]
                    );
            break;

        case PLX_IOCTL_GET_PORT_PROPERTIES:
            DebugPrint("PLX_IOCTL_GET_PORT_PROPERTIES\n");

            pIoBuffer->ReturnCode =
                PlxGetPortProperties(
                    pdx,
                    &(pIoBuffer->Key),
                    &(pIoBuffer->u.PortProp)
                    );
            break;


        /******************************************
         * Device Control Functions
         *****************************************/
        case PLX_IOCTL_PCI_DEVICE_RESET:
            DebugPrint("PLX_IOCTL_PCI_DEVICE_RESET\n");

            pIoBuffer->ReturnCode =
                PlxPciDeviceReset(
                    pdx,
                    &(pIoBuffer->Key)
                    );
            break;


        /******************************************
         * PCI Register Access Functions
         *****************************************/
        case PLX_IOCTL_PCI_REGISTER_READ:
            DebugPrint("PLX_IOCTL_PCI_REGISTER_READ\n");

            pIoBuffer->ReturnCode =
                PlxPciRegisterRead(
                    pIoBuffer->Key.bus,
                    pIoBuffer->Key.slot,
                    pIoBuffer->Key.function,
                    (U16)pIoBuffer->value[0],
                    (U32*)&(pIoBuffer->value[1])
                    );

            DebugPrint(
                "PCI Reg %03X = %08X\n",
                (U16)pIoBuffer->value[0],
                (U32)pIoBuffer->value[1]
                );
            break;

        case PLX_IOCTL_PCI_REGISTER_WRITE:
            DebugPrint("PLX_IOCTL_PCI_REGISTER_WRITE\n");

            pIoBuffer->ReturnCode =
                PlxPciRegisterWrite(
                    pIoBuffer->Key.bus,
                    pIoBuffer->Key.slot,
                    pIoBuffer->Key.function,
                    (U16)pIoBuffer->value[0],
                    (U32)pIoBuffer->value[1]
                    );

            DebugPrint(
                "Wrote %08X to PCI Reg %03X\n",
                (U32)pIoBuffer->value[1],
                (U16)pIoBuffer->value[0]
                );
            break;

        case PLX_IOCTL_PCI_REG_READ_UNSUPPORTED:
            DebugPrint("PLX_IOCTL_PCI_REG_READ_UNSUPPORTED\n");

            pIoBuffer->ReturnCode =
                PlxPciRegisterRead_Unsupported(
                    pIoBuffer->Key.bus,
                    pIoBuffer->Key.slot,
                    pIoBuffer->Key.function,
                    (U16)pIoBuffer->value[0],
                    (U32*)&(pIoBuffer->value[1])
                    );
            break;

        case PLX_IOCTL_PCI_REG_WRITE_UNSUPPORTED:
            DebugPrint("PLX_IOCTL_PCI_REG_WRITE_UNSUPPORTED\n");

            pIoBuffer->ReturnCode =
                PlxPciRegisterWrite_Unsupported(
                    pIoBuffer->Key.bus,
                    pIoBuffer->Key.slot,
                    pIoBuffer->Key.function,
                    (U16)pIoBuffer->value[0],
                    (U32)pIoBuffer->value[1]
                    );
            break;


        /******************************************
         * PLX-specific Register Access Functions
         *****************************************/
        case PLX_IOCTL_REGISTER_READ:
            DebugPrint("PLX_IOCTL_REGISTER_READ\n");

            pIoBuffer->value[1] =
                PlxRegisterRead(
                    pdx,
                    &(pIoBuffer->Key),
                    (U32)pIoBuffer->value[0],
                    &(pIoBuffer->ReturnCode),
                    TRUE        // Adjust offset based on port
                    );

            DebugPrint(
                "PLX Reg %03X = %08X\n",
                (U32)pIoBuffer->value[0],
                (U32)pIoBuffer->value[1]
                );
            break;

        case PLX_IOCTL_REGISTER_WRITE:

            pIoBuffer->ReturnCode =
                PlxRegisterWrite(
                    pdx,
                    &(pIoBuffer->Key),
                    (U32)pIoBuffer->value[0],
                    (U32)pIoBuffer->value[1],
                    TRUE        // Adjust offset based on port
                    );


            break;

        case PLX_IOCTL_MAPPED_REGISTER_READ:
            DebugPrint("PLX_IOCTL_MAPPED_REGISTER_READ\n");

            pIoBuffer->value[1] =
                PlxRegisterRead(
                    pdx,
                    &(pIoBuffer->Key),
                    (U32)pIoBuffer->value[0],
                    &(pIoBuffer->ReturnCode),
                    FALSE       // Don't adjust offset based on port
                    );

            DebugPrint(
                "PLX Mapped Reg %03X = %08X\n",
                (U32)pIoBuffer->value[0],
                (U32)pIoBuffer->value[1]
                );
            break;

        case PLX_IOCTL_MAPPED_REGISTER_WRITE:
            DebugPrint("PLX_IOCTL_MAPPED_REGISTER_WRITE\n");

            pIoBuffer->ReturnCode =
                PlxRegisterWrite(
                    pdx,
                    &(pIoBuffer->Key),
                    (U32)pIoBuffer->value[0],
                    (U32)pIoBuffer->value[1],
                    FALSE       // Don't adjust offset based on port
                    );

            DebugPrint(
                "Wrote %08X to PLX Mapped Reg %03X\n",
                (U32)pIoBuffer->value[1],
                (U32)pIoBuffer->value[0]
                );
            break;


        /******************************************
         * PCI Mapping Functions
         *****************************************/
        case PLX_IOCTL_PCI_BAR_PROPERTIES:
            DebugPrint("PLX_IOCTL_PCI_BAR_PROPERTIES\n");

            pIoBuffer->ReturnCode =
                PlxPciBarProperties(
                    pdx,
                    &(pIoBuffer->Key),
                    (U8)(pIoBuffer->value[0]),
                    &(pIoBuffer->u.BarProp)
                    );
            break;

        case PLX_IOCTL_PCI_BAR_MAP:
            DebugPrint("PLX_IOCTL_PCI_BAR_MAP\n");

            pIoBuffer->ReturnCode =
                PlxPciBarMap(
                    pdx,
                    &(pIoBuffer->Key),
                    (U8)(pIoBuffer->value[0]),
                    &(pIoBuffer->value[1]),
                    pOwner
                    );
            break;

        case PLX_IOCTL_PCI_BAR_UNMAP:
            DebugPrint("PLX_IOCTL_PCI_BAR_UNMAP\n");

            pIoBuffer->ReturnCode =
                PlxPciBarUnmap(
                    pdx,
                    &(pIoBuffer->Key),
                    PLX_INT_TO_PTR(pIoBuffer->value[1]),
                    pOwner
                    );
            break;


        /******************************************
         * Serial EEPROM Access Functions
         *****************************************/
        case PLX_IOCTL_EEPROM_PRESENT:
            DebugPrint("PLX_IOCTL_EEPROM_PRESENT\n");

            pIoBuffer->ReturnCode =
                PlxEepromPresent(
                    pdx,
                    &(pIoBuffer->Key),
                    (PLX_EEPROM_STATUS*)&(pIoBuffer->value[0])
                    );
            break;

        case PLX_IOCTL_EEPROM_PROBE:
            DebugPrint("PLX_IOCTL_EEPROM_PROBE\n");

            pIoBuffer->ReturnCode =
                PlxEepromProbe(
                    pdx,
                    &(pIoBuffer->Key),
                    (BOOLEAN*)&(pIoBuffer->value[0])
                    );
            break;

        case PLX_IOCTL_EEPROM_CRC_GET:
            DebugPrint("PLX_IOCTL_EEPROM_CRC_GET\n");

            pIoBuffer->ReturnCode =
                PlxEepromCrcGet(
                    pdx,
                    &(pIoBuffer->Key),
                    (U32*)&(pIoBuffer->value[0]),
                    (U8*)&(pIoBuffer->value[1])
                    );
            break;

        case PLX_IOCTL_EEPROM_CRC_UPDATE:
            DebugPrint("PLX_IOCTL_EEPROM_CRC_UPDATE\n");

            pIoBuffer->ReturnCode =
                PlxEepromCrcUpdate(
                    pdx,
                    &(pIoBuffer->Key),
                    (U32*)&(pIoBuffer->value[0]),
                    (BOOLEAN)pIoBuffer->value[1]
                    );
            break;

        case PLX_IOCTL_EEPROM_READ_BY_OFFSET:
            DebugPrint("PLX_IOCTL_EEPROM_READ_BY_OFFSET\n");

            pIoBuffer->ReturnCode =
                PlxEepromReadByOffset(
                    pdx,
                    &(pIoBuffer->Key),
                    (U16)pIoBuffer->value[0],
                    (U32*)&(pIoBuffer->value[1])
                    );

            DebugPrint(
                "EEPROM Offset %02X = %08X\n",
                (U16)pIoBuffer->value[0],
                (U32)pIoBuffer->value[1]
                );
            break;

        case PLX_IOCTL_EEPROM_WRITE_BY_OFFSET:
            DebugPrint("PLX_IOCTL_EEPROM_WRITE_BY_OFFSET\n");

            pIoBuffer->ReturnCode =
                PlxEepromWriteByOffset(
                    pdx,
                    &(pIoBuffer->Key),
                    (U16)pIoBuffer->value[0],
                    (U32)pIoBuffer->value[1]
                    );

            DebugPrint(
                "Wrote %08X to EEPROM Offset %02X\n",
                (U32)pIoBuffer->value[1],
                (U16)pIoBuffer->value[0]
                );
            break;

        case PLX_IOCTL_EEPROM_READ_BY_OFFSET_16:
            DebugPrintf_NoInfo(("PLX_IOCTL_EEPROM_READ_BY_OFFSET_16\n"));

            pIoBuffer->ReturnCode =
                PlxEepromReadByOffset_16(
                    pdx,
                    &(pIoBuffer->Key),
                    (U16)pIoBuffer->value[0],
                    (U16*)&(pIoBuffer->value[1])
                    );

            DebugPrint(
                "EEPROM Offset %02X = %04X\n",
                (U16)pIoBuffer->value[0],
                (U16)pIoBuffer->value[1]
                );
            break;

        case PLX_IOCTL_EEPROM_WRITE_BY_OFFSET_16:
            DebugPrintf_NoInfo(("PLX_IOCTL_EEPROM_WRITE_BY_OFFSET_16\n"));

            pIoBuffer->ReturnCode =
                PlxEepromWriteByOffset_16(
                    pdx,
                    &(pIoBuffer->Key),
                    (U16)pIoBuffer->value[0],
                    (U16)pIoBuffer->value[1]
                    );

            DebugPrint(
                "Wrote %04X to EEPROM Offset %02X\n",
                (U16)pIoBuffer->value[1],
                (U16)pIoBuffer->value[0]
                );
            break;


        /******************************************
         * I/O Port Access Functions
         *****************************************/
        case PLX_IOCTL_IO_PORT_READ:
            DebugPrint("PLX_IOCTL_IO_PORT_READ\n");

            pIoBuffer->ReturnCode =
                PlxPciIoPortTransfer(
                    pIoBuffer->value[0],
                    PLX_INT_TO_PTR(pIoBuffer->u.TxParams.u.UserVa),
                    pIoBuffer->u.TxParams.ByteCount,
                    (PLX_ACCESS_TYPE)pIoBuffer->value[1],
                    TRUE           // Specify read operation
                    );
            break;

        case PLX_IOCTL_IO_PORT_WRITE:
            DebugPrint("PLX_IOCTL_IO_PORT_WRITE\n");

            pIoBuffer->ReturnCode =
                PlxPciIoPortTransfer(
                    pIoBuffer->value[0],
                    PLX_INT_TO_PTR(pIoBuffer->u.TxParams.u.UserVa),
                    pIoBuffer->u.TxParams.ByteCount,
                    (PLX_ACCESS_TYPE)pIoBuffer->value[1],
                    FALSE          // Specify write operation
                    );
            break;


        /******************************************
         * Physical Memory Functions
         *****************************************/
        case PLX_IOCTL_PHYSICAL_MEM_ALLOCATE:
            DebugPrint("PLX_IOCTL_PHYSICAL_MEM_ALLOCATE\n");

            pIoBuffer->ReturnCode =
                PlxPciPhysicalMemoryAllocate(
                    pdx,
                    &(pIoBuffer->u.PciMemory),
                    (BOOLEAN)(pIoBuffer->value[0]),
                    pOwner
                    );
            break;

        case PLX_IOCTL_PHYSICAL_MEM_FREE:
            DebugPrint("PLX_IOCTL_PHYSICAL_MEM_FREE\n");

            pIoBuffer->ReturnCode =
                PlxPciPhysicalMemoryFree(
                    pdx,
                    &(pIoBuffer->u.PciMemory)
                    );
            break;

        case PLX_IOCTL_PHYSICAL_MEM_MAP:
            DebugPrint("PLX_IOCTL_PHYSICAL_MEM_MAP\n");

            pIoBuffer->ReturnCode =
                PlxPciPhysicalMemoryMap(
                    pdx,
                    &(pIoBuffer->u.PciMemory),
                    pOwner
                    );
            break;

        case PLX_IOCTL_PHYSICAL_MEM_UNMAP:
            DebugPrint("PLX_IOCTL_PHYSICAL_MEM_UNMAP\n");

            pIoBuffer->ReturnCode =
                PlxPciPhysicalMemoryUnmap(
                    pdx,
                    &(pIoBuffer->u.PciMemory),
                    pOwner
                    );
            break;

        case PLX_IOCTL_COMMON_BUFFER_PROPERTIES:
            DebugPrint("PLX_IOCTL_COMMON_BUFFER_PROPERTIES\n");

            pIoBuffer->ReturnCode = ApiSuccess;

            // Return buffer information
            if (pGbl_CommonBuffer == NULL)
            {
                pIoBuffer->u.PciMemory.PhysicalAddr = 0;
                pIoBuffer->u.PciMemory.CpuPhysical  = 0;
                pIoBuffer->u.PciMemory.Size         = 0;
            }
            else
            {
                pIoBuffer->u.PciMemory.PhysicalAddr =
                                      pGbl_CommonBuffer->BusPhysical;
                pIoBuffer->u.PciMemory.CpuPhysical =
                                      pGbl_CommonBuffer->CpuPhysical;
                pIoBuffer->u.PciMemory.Size =
                                      pGbl_CommonBuffer->Size;
            }
            break;


        /******************************************
         * Interrupt Support Functions
         *****************************************/
        case PLX_IOCTL_INTR_ENABLE:
            DebugPrint("PLX_IOCTL_INTR_ENABLE\n");

            pIoBuffer->ReturnCode =
                PlxInterruptEnable(
                    pdx,
                    &(pIoBuffer->u.PlxIntr)
                    );
            break;

        case PLX_IOCTL_INTR_DISABLE:
            DebugPrint("PLX_IOCTL_INTR_DISABLE\n");

            pIoBuffer->ReturnCode =
                PlxInterruptDisable(
                    pdx,
                    &(pIoBuffer->u.PlxIntr)
                    );
            break;

        case PLX_IOCTL_NOTIFICATION_REGISTER_FOR:
            DebugPrint("PLX_IOCTL_NOTIFICATION_REGISTER_FOR\n");

            pIoBuffer->ReturnCode =
                PlxNotificationRegisterFor(
                    pdx,
                    &(pIoBuffer->u.PlxIntr),
                    PLX_INT_TO_PTR(pIoBuffer->value[0]),
                    pOwner
                    );
            break;

        case PLX_IOCTL_NOTIFICATION_WAIT:
            DebugPrint("PLX_IOCTL_NOTIFICATION_WAIT\n");

            pIoBuffer->ReturnCode =
                PlxNotificationWait(
                    pdx,
                    PLX_INT_TO_PTR(pIoBuffer->value[0]),
                    (PLX_UINT_PTR)pIoBuffer->value[1]
                    );
            break;

        case PLX_IOCTL_NOTIFICATION_STATUS:
            DebugPrint("PLX_IOCTL_NOTIFICATION_STATUS\n");

            pIoBuffer->ReturnCode =
                PlxNotificationStatus(
                    pdx,
                    PLX_INT_TO_PTR(pIoBuffer->value[0]),
                    &(pIoBuffer->u.PlxIntr)
                    );
            break;

        case PLX_IOCTL_NOTIFICATION_CANCEL:
            DebugPrint("PLX_IOCTL_NOTIFICATION_CANCEL\n");

            pIoBuffer->ReturnCode =
                PlxNotificationCancel(
                    pdx,
                    PLX_INT_TO_PTR(pIoBuffer->value[0]),
                    pOwner
                    );
            break;


        /******************************************
         *         VPD Functions
         *****************************************/
        case PLX_IOCTL_VPD_READ:
            DebugPrint("PLX_IOCTL_VPD_READ\n");

            pIoBuffer->ReturnCode =
                PlxPciVpdRead(
                    pdx,
                    (U16)pIoBuffer->value[0],
                    (U32*)&(pIoBuffer->value[1])
                    );
            break;

        case PLX_IOCTL_VPD_WRITE:
            DebugPrint("PLX_IOCTL_VPD_WRITE\n");

            pIoBuffer->ReturnCode =
                PlxPciVpdWrite(
                    pdx,
                    (U16)pIoBuffer->value[0],
                    (U32)pIoBuffer->value[1]
                    );
            break;


        /******************************************
         * PCI Memory Space Transfer Functions
         *****************************************/
        case PLX_IOCTL_PCI_BAR_SPACE_READ:
            DebugPrint("PLX_IOCTL_PCI_BAR_SPACE_READ");

            pIoBuffer->ReturnCode =
                PlxPciBarSpaceTransfer(
                    pdx,
                    (U8)pIoBuffer->value[0],
                    (U32)pIoBuffer->u.TxParams.LocalAddr,
                    PLX_INT_TO_PTR(pIoBuffer->u.TxParams.u.UserVa),
                    pIoBuffer->u.TxParams.ByteCount,
                    (PLX_ACCESS_TYPE)pIoBuffer->value[1],
                    (BOOLEAN)pIoBuffer->value[2],
                    TRUE           // Specify read operation
                    );

			DebugPrint("PlxPciBarSpaceTransfer Completed");
			DebugPrint("PlxPciBarSpaceTransfer Completed");

            break;

        case PLX_IOCTL_PCI_BAR_SPACE_WRITE:
            DebugPrint("PLX_IOCTL_PCI_BAR_SPACE_WRITE\n");

            pIoBuffer->ReturnCode =
                PlxPciBarSpaceTransfer(
                    pdx,
                    (U8)pIoBuffer->value[0],
                    (U32)pIoBuffer->u.TxParams.LocalAddr,
                    PLX_INT_TO_PTR(pIoBuffer->u.TxParams.u.UserVa),
                    pIoBuffer->u.TxParams.ByteCount,
                    (PLX_ACCESS_TYPE)pIoBuffer->value[1],
                    (BOOLEAN)pIoBuffer->value[2],
                    FALSE          // Specify write operation
                    );
            break;


        /******************************************
         * DMA Functions
         *****************************************/
        case PLX_IOCTL_DMA_CONTROL:
            DebugPrint("PLX_IOCTL_DMA_CONTROL\n");

            pIoBuffer->ReturnCode =
                PlxChip_DmaControl(
                    pdx,
                    (U8)pIoBuffer->value[0],
                    (PLX_DMA_COMMAND)pIoBuffer->value[1]
                    );
            break;

        case PLX_IOCTL_DMA_STATUS:
            DebugPrint("PLX_IOCTL_DMA_STATUS\n");

            pIoBuffer->ReturnCode =
                PlxChip_DmaStatus(
                    pdx,
                    (U8)pIoBuffer->value[0]
                    );
            break;

        case PLX_IOCTL_DMA_CHANNEL_OPEN:
            DebugPrint("PLX_IOCTL_DMA_CHANNEL_OPEN\n");

            pIoBuffer->ReturnCode =
                PlxChip_DmaChannelOpen(
                    pdx,
                    (U8)pIoBuffer->value[0],
                    &(pIoBuffer->u.DmaProp),
                    pOwner
                    );
            break;

        case PLX_IOCTL_DMA_TRANSFER_BLOCK:
            DebugPrint("PLX_IOCTL_DMA_TRANSFER_BLOCK\n");

            pIoBuffer->ReturnCode =
                PlxChip_DmaTransferBlock(
                    pdx,
                    (U8)pIoBuffer->value[0],
                    &(pIoBuffer->u.TxParams)
                    );
            break;

        case PLX_IOCTL_DMA_TRANSFER_USER_BUFFER:
            DebugPrint("PLX_IOCTL_DMA_TRANSFER_USER_BUFFER\n");

            pIoBuffer->ReturnCode =
                PlxChip_DmaTransferUserBuffer(
                    pdx,
                    (U8)pIoBuffer->value[0],
                    &(pIoBuffer->u.TxParams)
                    );
            break;

        case PLX_IOCTL_DMA_CHANNEL_CLOSE:
            DebugPrint("PLX_IOCTL_DMA_CHANNEL_CLOSE\n");

            pIoBuffer->ReturnCode =
                PlxChip_DmaChannelClose(
                    pdx,
                    (U8)pIoBuffer->value[0],
                    TRUE
                    );
			
			break;

		/**************************************************
		*		USL Routines
		**************************************************/

		case IOCTL_SI10_SHARE_USER_STRUCT:
			DebugPrint("IOCTL_SI10_SHARE_USER_STRUCT");
			
			FreeUserStruct(pdx);

			pdx->pVirtualUserStruct=(struct SI10Data *)pStack->Parameters.DeviceIoControl.Type3InputBuffer;
			DebugPrint("Virtual Address = %x",pdx->pVirtualUserStruct);
			
			pdx->mdl = IoAllocateMdl(pdx->pVirtualUserStruct,sizeof (struct SI10Data),FALSE,FALSE,NULL);
			if(pdx->mdl) {
				MmProbeAndLockPages(pdx->mdl,KernelMode,IoModifyAccess);
				pdx->pSi10User=(struct SI10Data *)MmGetSystemAddressForMdl(pdx->mdl);
				DebugPrint("system Addrs = %x",pdx->pSi10User);
				
				for(ii=0;ii<10;ii++) {
					if(pdx->pSi10User->pAxis[ii]!=NULL){
						DebugPrint("Axis Addrs %d = %x",ii+1,pdx->pSi10User->pAxis[ii]);
						pdx->mdlAxis[ii] = IoAllocateMdl(pdx->pSi10User->pAxis[ii],sizeof (struct AxisData),FALSE,FALSE,NULL);
						
						if(pdx->mdlAxis[ii]) {
							MmProbeAndLockPages(pdx->mdlAxis[ii],KernelMode,IoModifyAccess);
							pdx->pAxis[ii]=(struct AxisData *)MmGetSystemAddressForMdl(pdx->mdlAxis[ii]);
							DebugPrint("system Axis Addrs = %x",pdx->pAxis[ii]);
						}
					} else {
						pdx->pAxis[ii] = NULL;
						DebugPrint("Axis = %d not initialized",ii);
					}
				}

			}

			pdx->bUserShared = TRUE;
			break;
		case IOCTL_SI10_LOAD_FPGA_CODE:
			if((InputLength!=0)||(pdx->pSi10User->pData==NULL)) {
			
			}
			else
			{
				FreeDataArray(pdx);
				DebugPrint("Virtual Data Array Address = %x",pdx->pSi10User->pData);
				pdx->mdlDataArray = IoAllocateMdl(pdx->pSi10User->pData,pdx->pSi10User->nDataSize,FALSE,FALSE,NULL);
				
				if(pdx->mdlDataArray!=NULL) {
					
					MmProbeAndLockPages(pdx->mdlDataArray,KernelMode,IoModifyAccess);
					pdx->pDataArray=(char *)MmGetSystemAddressForMdl(pdx->mdlDataArray);
					DebugPrint("system Addrs = %x %d",pdx->pDataArray,pdx->pSi10User->nDataSize);	
					pData = pdx->pDataArray;
					

					// Set Program- & Data & Clk to Low
					PLX_9000_REG_WRITE(pdx,0x00000054,0x02480482);
					
					// Set Program- to high Data & Clk to Low
					PLX_9000_REG_WRITE(pdx,0x00000054,0x02480482 | 0x00000004);


					ii=0;
					while( PLX_9000_REG_READ(pdx,0x00000054) != 0x00000020) {
						ii++;
						if(ii==2000) {
							DebugPrint("Read Reg Failed when loading FPGA");
							break;
						}
					};

					//Write data
					for(ii=0;ii<pdx->pSi10User->nDataSize;ii++) {
						Data = *pData;
						for(bit=0;bit<8;bit++) {
							Value = (0x00000000 | (((Data<<bit) & 0x80) << 1)) | 0x02480482 | 0x00000004;
							PLX_9000_REG_WRITE(pdx,0x00000054,Value);
							PLX_9000_REG_WRITE(pdx,0x00000054,Value | 0x00000800); //Clk bit
							PLX_9000_REG_WRITE(pdx,0x00000054,Value);

						}
						pData++;
					}

					// Set GPIO 2 to input to release bit 0 of on-board micro-controller data bus to which it is connected 
					// NB Must keep PROG bit set high at all times substequent to completing FPGA configuration to prevent
					// FPGA from clearing itself.
					PLX_9000_REG_WRITE(pdx,0x00000054, 0x02480402 | 0x00000004);

					FreeDataArray(pdx);

					DebugPrint("Bytes Written %d, nDataSize %d",ii,pdx->pSi10User->nDataSize);
				};
			};
			break;

		case IOCTL_SI10_ENABLE_INTERRUPT_CALLBACK:

			if (InputLength < sizeof(HANDLE))
			{
				DebugPrint("IOCTL_SI10_ENABLE_INTERRUPT_CALLBACK - STATUS_INVALID_PARAMETER");
				break;
			}
			
			if (pdx->pIntEvent)
			{						// event already registered
				ObDereferenceObject(pdx->pIntEvent);
				pdx->pIntEvent = NULL;
			}						// event already registered

			status = ObReferenceObjectByHandle(* (PHANDLE) pStack->Parameters.DeviceIoControl.Type3InputBuffer, EVENT_MODIFY_STATE, *ExEventObjectType, pIrp->RequestorMode, (PVOID*) &pdx->pIntEvent, NULL);
	
			break;
			
					
		case IOCTL_SI10_SHARE_TIMESLOT_STRUCT:
			DebugPrint("IOCTL_SI10_SHARE_TIMESLOT_STRUCT");

			FreeTimeSlot(pdx); 

			pdx->pVirtualTimeSlot=(struct TimeSlotData *)pStack->Parameters.DeviceIoControl.Type3InputBuffer;
			DebugPrint("Virtual Tiemslot Address = %x",pdx->pVirtualTimeSlot);
			
			pdx->mdlTimeSlot = IoAllocateMdl(pdx->pVirtualTimeSlot,(sizeof (struct TimeSlotData))*8,FALSE,FALSE,NULL);
			if(pdx->mdlTimeSlot) {
				MmProbeAndLockPages(pdx->mdlTimeSlot,KernelMode,IoModifyAccess);
				pdx->pTimeSlotUser=(struct TimeSlotData *)MmGetSystemAddressForMdl(pdx->mdlTimeSlot);
				DebugPrint("system Addrs = %x",pdx->pTimeSlotUser);
			};

			pdx->bTimeSlotShared = TRUE;
			break;

		case IOCTL_SI10_SHARE_UT_USER_STRUCT:
			DebugPrint("IOCTL_SI10_SHARE_UT_USER_STRUCT");
		
			FreeUtUserStruct(pdx);
			
			pdx->pVirtualUtUserStruct=(struct SI10UTData *)pStack->Parameters.DeviceIoControl.Type3InputBuffer;
			DebugPrint("Virtual UT Address = %x",pdx->pVirtualUtUserStruct);
			
			pdx->utmdl = IoAllocateMdl(pdx->pVirtualUtUserStruct,sizeof (struct SI10UTData),FALSE,FALSE,NULL);
			if(pdx->utmdl) {
				MmProbeAndLockPages(pdx->utmdl,KernelMode,IoModifyAccess);
				pdx->pUtUser=(struct SI10UTData *)MmGetSystemAddressForMdl(pdx->utmdl);
				DebugPrint("system Ut Addrs = %x",pdx->pUtUser);
			}

			pdx->bUTShared = TRUE;

			nSize = pdx->pUtUser->nSamplesLine * pdx->pUtUser->nNumberLines;
			DebugPrint("nSize = %d",nSize);

			for(ii=0;ii<pdx->pUtUser->nAmpImages;ii++) 
			{
				DebugPrint("Virtual Data Array Address = %x",pdx->pUtUser->m_cAmpBuffer[ii]);
				pdx->mdlAmpBuffer[ii] = IoAllocateMdl(pdx->pUtUser->m_cAmpBuffer[ii],nSize,FALSE,FALSE,NULL);
				if(pdx->mdlAmpBuffer[ii]) {
					MmProbeAndLockPages(pdx->mdlAmpBuffer[ii],KernelMode,IoModifyAccess);
					pdx->pAmpBuffer[ii]=(unsigned char *)MmGetSystemAddressForMdl(pdx->mdlAmpBuffer[ii]);
					DebugPrint("system Addrs = %x %d",pdx->pAmpBuffer[ii],nSize);
				};

			}

			nSize *=4; //char to int
			for(ii=0;ii<pdx->pUtUser->nTimeImages;ii++) 
			{				
				DebugPrint("Virtual Data Array Address = %x",pdx->pUtUser->m_nTimeBuffer[ii]);
				pdx->mdlTimeBuffer[ii] = IoAllocateMdl(pdx->pUtUser->m_nTimeBuffer[ii],nSize,FALSE,FALSE,NULL);
				if(pdx->mdlTimeBuffer[ii]) {
					MmProbeAndLockPages(pdx->mdlTimeBuffer[ii],KernelMode,IoModifyAccess);
					pdx->pTimeBuffer[ii]=(unsigned int *)MmGetSystemAddressForMdl(pdx->mdlTimeBuffer[ii]);
					DebugPrint("system Addrs = %x %d",pdx->pTimeBuffer[ii],nSize);
				};
			}

		
			
			DebugPrint("nSamplesLine = %d",pdx->pUtUser->nSamplesLine);
			DebugPrint("nNumberLines = %d",pdx->pUtUser->nNumberLines);
			DebugPrint("nStartPos = %d",pdx->pUtUser->nStartPos);
			DebugPrint("nFinishPos = %d",pdx->pUtUser->nFinishPos);
			DebugPrint("nStepSize * 1000 = %d",pdx->pUtUser->nStepSize);
			DebugPrint("nSamplePeriod = %d",pdx->pUtUser->nSamplePeriod);
			DebugPrint("pbCollectData = %d",pdx->pUtUser->bCollectData);
			DebugPrint("nAmpImages = %d",pdx->pUtUser->nAmpImages);


			break;
		case IOCTL_SI10_FREE_UT_USER_STRUCT:
			
			FreeUtUserStruct(pdx);

			break;

        /******************************************
         * Unsupported Messages
         *****************************************/
        default:
            DebugPrint(
                "Unsupported PLX_IOCTL_Xxx (0x%x)",
                pStack->Parameters.DeviceIoControl.IoControlCode
                );

            pIoBuffer->ReturnCode = ApiUnsupportedFunction;
            break;
    }

_Exit_Dispatch_IoControl:
    return PlxCompleteIrp(
               pIrp,
               STATUS_SUCCESS
               );
}


