#ifndef __PLXCHIP_H
#define __PLXCHIP_H

/******************************************************************************
 *
 * File Name:
 *
 *      PlxChip.h
 *
 * Description:
 *
 *      This file contains definitions specific to a PLX Chip
 *
 * Revision History:
 *
 *      06-01-08 : PLX SDK v6.00
 *
 ******************************************************************************/


#if defined(PCI9050)
    #include "Reg9050.h"
#elif defined(PCI9030)
    #include "Reg9030.h"
#elif defined(PCI9080)
    #include "Reg9080.h"
#elif defined(PCI9054)
    #include "Reg9054.h"
#elif defined(PCI9056)
    #include "Reg9056.h"
#elif defined(PCI9656)
    #include "Reg9656.h"
#elif defined(PCI8311)
    #include "Reg8311.h"
#endif




/**********************************************
 *               Definitions
 *********************************************/
#if defined(PCI9050)

    #define PLX_CHIP_TYPE                       0x9050
    #define PLX_DRIVER_NAME                     "Plx9050"
    #define PLX_DRIVER_NAME_UNICODE             L"Plx9050"
    #define PLX_LOG_FILE_UNICODE                L"\\SystemRoot\\Plx9050.txt" // Log file name
    #define MAX_PLX_REG_OFFSET                  PCI9050_MAX_REG_OFFSET       // Max PLX register offset
    #define DEFAULT_SIZE_COMMON_BUFFER          0                            // Default size of Common Buffer
    #define PLX_CHIP_CONTEXT_TOTAL_REGS         (0x54 / sizeof(U32))         // Number of regs to save/restore during power change

#elif defined(PCI9030)

    #define PLX_CHIP_TYPE                       0x9030
    #define PLX_DRIVER_NAME                     "Plx9030"
    #define PLX_DRIVER_NAME_UNICODE             L"Plx9030"
    #define PLX_LOG_FILE_UNICODE                L"\\SystemRoot\\Plx9030.txt" // Log file name
    #define MAX_PLX_REG_OFFSET                  PCI9030_MAX_REG_OFFSET       // Max PLX register offset
    #define DEFAULT_SIZE_COMMON_BUFFER          0                            // Default size of Common Buffer
    #define PLX_CHIP_CONTEXT_TOTAL_REGS         (0x58 / sizeof(U32))         // Number of regs to save/restore during power change

#elif defined(PCI9080)

    #define PLX_CHIP_TYPE                       0x9080
    #define PLX_DRIVER_NAME                     "Plx9080"
    #define PLX_DRIVER_NAME_UNICODE             L"Plx9080"
    #define PLX_LOG_FILE_UNICODE                L"\\SystemRoot\\Plx9080.txt" // Log file name
    #define MAX_PLX_REG_OFFSET                  PCI9080_MAX_REG_OFFSET       // Max PLX register offset
    #define DEFAULT_SIZE_COMMON_BUFFER          (64 * 1024)                  // Default size of Common Buffer
    #define NUMBER_OF_DMA_CHANNELS              2                            // Total number of DMA Channels
    #define PLX_CHIP_CONTEXT_TOTAL_REGS         (0xFC / sizeof(U32))         // Number of regs to save/restore during power change

#elif defined(PCI9054)

    #define PLX_CHIP_TYPE                       0x9054
    #define PLX_DRIVER_NAME                     "Plx9054"
    #define PLX_DRIVER_NAME_UNICODE             L"Plx9054"
    #define PLX_LOG_FILE_UNICODE                L"\\SystemRoot\\Plx9054.txt" // Log file name
    #define MAX_PLX_REG_OFFSET                  PCI9054_MAX_REG_OFFSET       // Max PLX register offset
    #define DEFAULT_SIZE_COMMON_BUFFER          (64 * 1024)                  // Default size of Common Buffer
    #define NUMBER_OF_DMA_CHANNELS              2                            // Total number of DMA Channels
    #define PLX_CHIP_CONTEXT_TOTAL_REGS         (0x100 / sizeof(U32))        // Number of regs to save/restore during power change

#elif defined(PCI9056)

    #define PLX_CHIP_TYPE                       0x9056
    #define PLX_DRIVER_NAME                     "Plx9056"
    #define PLX_DRIVER_NAME_UNICODE             L"Plx9056"
    #define PLX_LOG_FILE_UNICODE                L"\\SystemRoot\\Plx9056.txt" // Log file name
    #define MAX_PLX_REG_OFFSET                  PCI9056_MAX_REG_OFFSET       // Max PLX register offset
    #define DEFAULT_SIZE_COMMON_BUFFER          (64 * 1024)                  // Default size of Common Buffer
    #define NUMBER_OF_DMA_CHANNELS              2                            // Total number of DMA Channels
    #define PLX_CHIP_CONTEXT_TOTAL_REGS         (0x108 / sizeof(U32))        // Number of regs to save/restore during power change

#elif defined(PCI9656)

    #define PLX_CHIP_TYPE                       0x9656
    #define PLX_DRIVER_NAME                     "Plx9656"
    #define PLX_DRIVER_NAME_UNICODE             L"Plx9656"
    #define PLX_LOG_FILE_UNICODE                L"\\SystemRoot\\Plx9656.txt" // Log file name
    #define MAX_PLX_REG_OFFSET                  PCI9656_MAX_REG_OFFSET       // Max PLX register offset
    #define DEFAULT_SIZE_COMMON_BUFFER          (64 * 1024)                  // Default size of Common Buffer
    #define NUMBER_OF_DMA_CHANNELS              2                            // Total number of DMA Channels
    #define PLX_CHIP_CONTEXT_TOTAL_REGS         (0x108 / sizeof(U32))        // Number of regs to save/restore during power change

#elif defined(PCI8311)

    #define PLX_CHIP_TYPE                       0x8311
    #define PLX_DRIVER_NAME                     "Plx8311"
    #define PLX_DRIVER_NAME_UNICODE             L"Plx8311"
    #define PLX_LOG_FILE_UNICODE                L"\\SystemRoot\\Plx8311.txt" // Log file name
    #define MAX_PLX_REG_OFFSET                  PCI8311_MAX_REG_OFFSET       // Max PLX register offset
    #define DEFAULT_SIZE_COMMON_BUFFER          (64 * 1024)                  // Default size of Common Buffer
    #define NUMBER_OF_DMA_CHANNELS              2                            // Total number of DMA Channels
    #define PLX_CHIP_CONTEXT_TOTAL_REGS         (0x108 / sizeof(U32))        // Number of regs to save/restore during power change

#endif



/***********************************************************
 * The following definition is required for drivers
 * requiring DMA functionality.
 **********************************************************/
#if defined(NUMBER_OF_DMA_CHANNELS)
    #define PLX_DMA_SUPPORT
#else
    // No DMA support
#endif



#endif
