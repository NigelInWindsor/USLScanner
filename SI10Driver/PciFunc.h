#ifndef __PLX_PCI_FUNCTIONS_H
#define __PLX_PCI_FUNCTIONS_H

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
 *      PciFunc.h
 *
 * Description:
 *
 *      The header file for PCI support functions
 *
 * Revision History:
 *
 *      12-01-07 : PLX SDK v5.20
 *
 ******************************************************************************/


#include "DrvDefs.h"




/**********************************************
 *               Definitions
 *********************************************/
#define PLX_PCI_REG_READ(pdx, offset, pValue) \
    PciRegisterRead_UsingPnP( \
        (pdx)->pDeviceObject, \
        (U16)(offset),        \
        (pValue),             \
        sizeof(U32)           \
        )

#define PLX_PCI_REG_WRITE(pdx, offset, value) \
    PciRegisterWrite_UsingPnP( \
        (pdx)->pDeviceObject,  \
        (U16)(offset),         \
        (value)                \
        )


#define PlxPciRegisterRead              PlxPciRegisterRead_Unsupported
#define PlxPciRegisterWrite             PlxPciRegisterWrite_Unsupported


// Memory access functions
#define PHYS_MEM_READ_8(addr)           ((U8)PlxPhysicalMemRead((addr) , sizeof(U8)))
#define PHYS_MEM_READ_16(addr)          ((U16)PlxPhysicalMemRead((addr), sizeof(U16)))
#define PHYS_MEM_READ_32(addr)          ((U32)PlxPhysicalMemRead((addr), sizeof(U32)))

#define PHYS_MEM_WRITE_8(addr , value)  PlxPhysicalMemWrite((addr), (value), sizeof(U8))
#define PHYS_MEM_WRITE_16(addr, value)  PlxPhysicalMemWrite((addr), (value), sizeof(U16))
#define PHYS_MEM_WRITE_32(addr, value)  PlxPhysicalMemWrite((addr), (value), sizeof(U32))


// Used to scan ROM for services
#define BIOS_MEM_START                  0x000E0000
#define BIOS_MEM_END                    0x000FFFF0

// ACPI probe states
#define ACPI_PCIE_NOT_PROBED            0
#define ACPI_PCIE_NOT_AVAILABLE         (-1)


// ACPI RSDT v1.0 structure
typedef struct _ACPI_RSDT_v1_0
{
    U32 Signature;
    U32 Length;
    U8  Revision;
    U8  Oem_Id[6];
    U8  Oem_Table_Id[8];
    U32 Oem_Revision;
    U32 Creator_Id;
    U32 Creator_Revision;
} ACPI_RSDT_v1_0;




/**********************************************
 *               Functions
 *********************************************/
PLX_STATUS
PciRegisterRead_UsingPnP(
    DEVICE_OBJECT *fdo,
    U16            offset,
    VOID          *pBuffer,
    U8             size
    );

PLX_STATUS
PciRegisterWrite_UsingPnP(
    DEVICE_OBJECT *fdo,
    U16            offset,
    U32            value
    );

PLX_STATUS
PlxPciRegisterRead_Unsupported(
    U8   bus,
    U8   slot,
    U8   function,
    U16  offset,
    U32 *pValue
    );

PLX_STATUS
PlxPciRegisterWrite_Unsupported(
    U8  bus,
    U8  slot,
    U8  function,
    U16 offset,
    U32 value
    );

PLX_STATUS
PlxPciExpressRegRead(
    U8   bus,
    U8   slot,
    U8   function,
    U16  offset,
    U32 *pValue
    );

PLX_STATUS
PlxPciExpressRegWrite(
    U8  bus,
    U8  slot,
    U8  function,
    U16 offset,
    U32 value
    );

VOID
PlxProbeForEcamBase(
    VOID
    );

U32
PlxPhysicalMemRead(
    U32 address,
    U8  size
    );

U32
PlxPhysicalMemWrite(
    U32 address,
    U32 value,
    U8  size
    );

NTSTATUS
GetBusSlotNumber(
    DEVICE_OBJECT    *pdo,
    DEVICE_EXTENSION *pdx,
    U32               CompareValue
    );



#endif
