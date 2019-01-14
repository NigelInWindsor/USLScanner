#ifndef __DRIVER_DEFS_H
#define __DRIVER_DEFS_H

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
 *      DrvDefs.h
 *
 * Description:
 *
 *      Common definitions used in the driver
 *
 * Revision History:
 *
 *      06-01-08 : PLX SDK v6.00
 *
 ******************************************************************************/


#include "Plx.h"
#include "PlxChip.h"
#include "PlxTypes.h"




/**********************************************
 *               Definitions
 *********************************************/
#define PLX_MAX_NAME_LENGTH                 0x20          // Max length of registered device name
#define MIN_WORKING_POWER_STATE             PowerDeviceD2 // Minimum state required for local register access

// Used for build of SGL descriptors
#define SGL_DESC_IDX_PCI_LOW                0
#define SGL_DESC_IDX_LOC_ADDR               1
#define SGL_DESC_IDX_COUNT                  2
#define SGL_DESC_IDX_NEXT_DESC              3
#define SGL_DESC_IDX_PCI_HIGH               4

// Used to dump SGL descriptors in debug mode  (0 = Do Not Display   1 = Display SGL Descriptors)
#if defined(PLX_DISPLAY_SGL)
    #define PLX_DEBUG_DISPLAY_SGL_DESCR     1
#else
    #define PLX_DEBUG_DISPLAY_SGL_DESCR     0
#endif



// Uncomment PLX_LOG_TO_FILE definition below to log to file instead of kernel debugger
//#define PLX_LOG_TO_FILE                     1

// Macros to support Kernel-level logging in Debug builds
#if defined(PLX_LOG_TO_FILE)
    #define _PlxDbgFunc                     PlxLogPrintf
#else
    #define _PlxDbgFunc                     DbgPrint
#endif

#if defined(PLX_DEBUG)
    #define DebugPrintf(arg)                _Debug_Print_Macro(arg)
    #define DebugPrintf_NoInfo(arg)         _PlxDbgFunc arg
    #define ErrorPrintf(arg)                _Debug_Print_Macro(arg)
#else
    #define DebugPrintf(arg)
    #define DebugPrintf_NoInfo(arg)
    #define ErrorPrintf(arg)                _Debug_Print_Macro(arg)
#endif

#define _Debug_Print_Macro(arg)            \
    do                                     \
    {                                      \
        _PlxDbgFunc(PLX_DRIVER_NAME ": "); \
        _PlxDbgFunc arg;                   \
    }                                      \
    while (0)



// Macros for Memory access to/from user-space addresses
#define DEV_MEM_TO_USER_8( VaUser, VaDev, count)    READ_REGISTER_BUFFER_UCHAR(  (UCHAR*)(VaDev),  (UCHAR*)(VaUser),  (count))
#define DEV_MEM_TO_USER_16(VaUser, VaDev, count)    READ_REGISTER_BUFFER_USHORT((USHORT*)(VaDev), (USHORT*)(VaUser),  (count) >> 1)
#define DEV_MEM_TO_USER_32(VaUser, VaDev, count)    READ_REGISTER_BUFFER_ULONG(  (ULONG*)(VaDev),  (ULONG*)(VaUser),  (count) >> 2)
#define USER_TO_DEV_MEM_8( VaDev, VaUser, count)    WRITE_REGISTER_BUFFER_UCHAR(  (UCHAR*)(VaDev),  (UCHAR*)(VaUser), (count))
#define USER_TO_DEV_MEM_16(VaDev, VaUser, count)    WRITE_REGISTER_BUFFER_USHORT((USHORT*)(VaDev), (USHORT*)(VaUser), (count) >> 1)
#define USER_TO_DEV_MEM_32(VaDev, VaUser, count)    WRITE_REGISTER_BUFFER_ULONG(  (ULONG*)(VaDev),  (ULONG*)(VaUser), (count) >> 2)


// Macros for I/O port access
#define IO_PORT_READ_8(port)                        READ_PORT_UCHAR (  (UCHAR*)(port))
#define IO_PORT_READ_16(port)                       READ_PORT_USHORT( (USHORT*)(port))
#define IO_PORT_READ_32(port)                       READ_PORT_ULONG (  (ULONG*)(port))
#define IO_PORT_WRITE_8(port, val)                  WRITE_PORT_UCHAR(  (UCHAR*)(port),  (UCHAR)(val))
#define IO_PORT_WRITE_16(port, val)                 WRITE_PORT_USHORT((USHORT*)(port), (USHORT)(val))
#define IO_PORT_WRITE_32(port, val)                 WRITE_PORT_ULONG(  (ULONG*)(port),  (ULONG)(val))


// Macros for device memory access
#define DEV_MEM_READ_8                              READ_REGISTER_UCHAR
#define DEV_MEM_READ_16                             READ_REGISTER_USHORT
#define DEV_MEM_READ_32                             READ_REGISTER_ULONG
#define DEV_MEM_WRITE_8                             WRITE_REGISTER_UCHAR
#define DEV_MEM_WRITE_16                            WRITE_REGISTER_USHORT
#define DEV_MEM_WRITE_32                            WRITE_REGISTER_ULONG



// Macros for PLX chip register access
#define PLX_9000_REG_READ(pdx, offset)                   \
    DEV_MEM_READ_32(                                     \
        (U32*)(((U8*)((pdx)->PciBar[0].pVa)) + (offset)) \
        )

#define PLX_9000_REG_WRITE(pdx, offset, value)            \
    DEV_MEM_WRITE_32(                                     \
        (U32*)(((U8*)((pdx)->PciBar[0].pVa)) + (offset)), \
        (value)                                           \
        )




/***********************************************************
 * The following definition sets the maximum size that an
 * MDL can describe.  Although not documented in the DDK
 * until recently, the memory allocated for an MDL is 64k,
 * which limits the number of physical page entries allowed
 * after the MDL.
 *
 * Assuming 4k page sizes, the limit calculation yields a
 * size of just under 64MB on a 32-bit system.  On a system with
 * 64-bit addressing, the max size per MDL is less than 32MB.
 * The calculation for determining the max size is:
 *
 *   Total Mem = PAGE_SIZE * ((64k - sizeof(MDL)) / sizeof(PLX_UINT_PTR))
 *
 * The total size is decremented by an additional amount because
 * Windows may still fail mappings in certain cases if the size
 * MAX_MDL_SIZE is too large.
 **********************************************************/
#define MAX_MDL_SIZE                        (PAGE_SIZE * ((65535 - sizeof(MDL)) / sizeof(PLX_UINT_PTR))) - (1 << 21)



/***********************************************************
 * The following definition sets the maximum number of
 * consecutive virtual addresses for a PCI BAR space.  Since
 * individual MDLs are limited to 64MB, when mapping a space
 * to obtain a user-mode virtual address, each individual
 * MDL for a space must be mapped separately.  The virtual
 * addresses must be saved for later unmapping.
 *
 * The following calculation determines the theoretical
 * limitation of a user mapping for a PCI space:
 *
 *   Max mapping size = (Max # virt addresses) * (Max MDL size)
 * 
 * If the max virtual address count is 5, for example, and
 * each MDL is limited to 60MB, the theoretical limit that the
 * driver can map will be a 300MB PCI space.
 *
 * NOTE: Increasing this number has no effect if a mapping
 *       fails due to system resource constraints.  It is
 *       primarily used to store multiple addresses so they
 *       can later be unmapped properly.
 **********************************************************/
#define MAX_VIRTUAL_ADDR                    10      // Max number of virtual addresses for a space



/***********************************************************
 * The following definition determines the maximum size
 * of all contiguous page-locked buffers the driver may try
 * to allocate.
 *
 * The driver allocates a DMA adapter object for each
 * device it owns.  When the adapter object is allocated,
 * with IoGetDmaAdapter, the maximum DMA transfer length must
 * be specified.  During creation of the DMA adapter object,
 * the OS reserves enough resources (i.e. map registers) to
 * handle simultaneously allocated buffers up to the size
 * specified in IoGetDmaAdapter.  Since the DMA Adapter
 * object is used to allocate common buffers, the total
 * size is limited to the DMA length used in IoGetDmaAdapter.
 *
 * In other words, if IoGetDmaAdapter is called with a DMA
 * length of 8MB, the driver can allocate, for example, only
 * two 4MB buffers.  A call to allocate an additional buffer
 * will fail since the 8MB limit is reached.  As a result,
 * the definition below can be set to a large number, such
 * as 100MB, so as not to severely limit buffer allocation.
 *
 * NOTE: The OS will only reserve enough resources to handle
 *       a large-enough buffer.  It will NOT actually reserve
 *       memory.  Available memory is determined at the time
 *       of the allocation request and requests may still
 *       fail, depending upon system resources.
 **********************************************************/
#define PHYS_MEM_MAX_SIZE_ALL               (100 << 20)     // Max total allowable size of physically memory to allocate



// Information stored in Registry
typedef struct _PLX_REGISTRY_INFO
{
    U32 Size_CommonBuffer;
} PLX_REGISTRY_INFO;


// PCI Interrupt wait object
typedef struct _PLX_WAIT_OBJECT
{
    LIST_ENTRY  ListEntry;
    VOID       *pOwner;
    U32         Notify_Flags;                   // Registered interrupt(s) for notification
    U32         Notify_Doorbell;                // Registered doorbell interrupt(s) for notification
    U32         Source_Ints;                    // Interrupt(s) that caused notification
    U32         Source_Doorbell;                // Doorbells that caused notification
    PKEVENT     pKEvent;
} PLX_WAIT_OBJECT;


// Argument for interrupt source access functions
typedef struct _PLX_INTERRUPT_DATA
{
    struct _DEVICE_EXTENSION *pdx;
    U32                       Source_Ints;
    U32                       Source_Doorbell;
} PLX_INTERRUPT_DATA;


// Information about contiguous, page-locked buffers
typedef struct _PLX_PHYS_MEM_OBJECT
{
    LIST_ENTRY   ListEntry;
    VOID        *pOwner;
    VOID        *pKernelVa;
    U64          CpuPhysical;                   // CPU Physical Address
    U64          BusPhysical;                   // Bus Physical Address
    U32          Size;                          // Buffer size
    PMDL         pMdl;
    BOOLEAN      bCacheable;
    LIST_ENTRY   List_Mappings;                 // List of mappings for this physical memory
    KSPIN_LOCK   Lock_MappingsList;             // Spinlock for mappings list
} PLX_PHYS_MEM_OBJECT;


// Physical memory user mapping
typedef struct _PLX_USER_MAPPING
{
    LIST_ENTRY  ListEntry;
    VOID       *pOwner;
    U8          BarIndex;
    VOID       *pUserVa[MAX_VIRTUAL_ADDR];
} PLX_USER_MAPPING;


// PCI BAR Space information
typedef struct _PLX_PCI_BAR_INFO
{
    U32              *pVa;                      // BAR Kernel Virtual Address
    PLX_PCI_BAR_PROP  Properties;               // BAR Properties
    PMDL              pMdl;                     // MDL for the BAR space
} PLX_PCI_BAR_INFO;


// DMA channel information 
typedef struct _PLX_DMA_INFO
{
    VOID                *pOwner;                // Object that requested to open the channel
    BOOLEAN              bOpen;                 // Flag to note if DMA channel is open
    BOOLEAN              bSglPending;           // Flag to note if an SGL DMA is pending
    BOOLEAN              bLocalAddrConstant;    // Flag to keep track if local address remains constant
    PMDL                 pMdl;                  // MDL of the user buffer for locking and unlocking
    PLX_PHYS_MEM_OBJECT  SglBuffer;             // Current SGL descriptor list buffer
} PLX_DMA_INFO;


// Argument for ISR synchronized register access
typedef struct _PLX_REG_DATA
{
    struct _DEVICE_EXTENSION *pdx;
    U32                       offset;
    U32                       BitsToSet;
    U32                       BitsToClear;
} PLX_REG_DATA;


// All relevant information about the device
typedef struct _DEVICE_EXTENSION
{
    DEVICE_OBJECT      *pDeviceObject;              // Device object this extension belongs to
    DMA_ADAPTER        *pDmaAdapter;                // DMA Adapter object for allocating physical memory
    DEVICE_OBJECT      *pLowerDeviceObject;
    DEVICE_OBJECT      *pPhysicalDeviceObject;

    PLX_DEVICE_KEY      Key;                        // Device location & identification
    U32                 PlxChipType;
    U8                  PlxRevision;
    WCHAR               LinkName[PLX_MAX_NAME_LENGTH];
    PLX_PCI_BAR_INFO    PciBar[PCI_NUM_BARS_TYPE_00];

    DEVICE_POWER_STATE  PowerState;                 // Power management information
    BOOLEAN             bSaveContext;
    U32                *pSavedContext;

	/********* USL Variables **************/
	PMDL					mdl;
	struct SI10Data			*pVirtualUserStruct;
	struct SI10Data			*pSi10User;			//System Addrs
	BOOLEAN					bUserShared;

	PMDL					mdlAxis[10]; //Axis Data
	struct	AxisData		*pAxis[10];

	PMDL					mdlDataArray; //Data Pointer within SI10 Struct, used for FPGA
	char					*pDataArray;
	
	PKEVENT					pIntEvent;
		
	PMDL					mdlTimeSlot;		//TimeSlotData
	struct TimeSlotData		*pVirtualTimeSlot;
	struct TimeSlotData		*pTimeSlotUser;
	BOOLEAN					bTimeSlotShared;

	PMDL					utmdl;				//UtUserData
	struct SI10UTData		*pVirtualUtUserStruct;
	struct SI10UTData		*pUtUser;			//System Addrs
	int						*pnSamplesLine;
	BOOLEAN					bUTShared;

	PMDL					mdlAmpBuffer[100];
	unsigned char			*pAmpBuffer[100];			//Amp data Buffer

	PMDL					mdlTimeBuffer[100];
	unsigned int			*pTimeBuffer[100];			//Time Data Buffer


    // Hardware locking parameters
    KSPIN_LOCK          Lock_HwAccess;

    PKINTERRUPT         pInterruptObject;
    KDPC                DpcForIsr;

    LIST_ENTRY          List_BarMappings;           // List of mappings to user space
    KSPIN_LOCK          Lock_BarMappingsList;       // Spinlock for user mappings list

    LIST_ENTRY          List_PhysicalMem;           // List of user-allocated physical memory
    KSPIN_LOCK          Lock_PhysicalMemList;       // Spinlock for physical memory list

    LIST_ENTRY          List_WaitObjects;           // List of registered notification objects
    KSPIN_LOCK          Lock_WaitObjectsList;       // Spinlock for notification objects list

#if defined(PLX_DMA_SUPPORT)                        // DMA channel information
    PLX_DMA_INFO        DmaInfo[NUMBER_OF_DMA_CHANNELS];
    KSPIN_LOCK          Lock_DmaChannel;            // Spinlock for DMA channel access
#endif

} DEVICE_EXTENSION;



#endif
