/******************************************************************************

  Copyright (C) 2010 [Hilscher Gesellschaft für Systemautomation mbH]

  This program can be used by everyone according to the netX Open Source
  Software license. The license agreement can be downloaded from
  http://www.industrialNETworX.com

*******************************************************************************

  Last Modification:
    @version $Id: rX_Results.h 333 2010-04-26 09:32:25Z Volker $

  Description:
    Return Codes of RX-Functions

  Changes:
    Date        Author        Description
  ---------------------------------------------------------------------------
    2010-03-23  NC            File created.

******************************************************************************/


#ifndef __RX_RESULTS_H
  #define __RX_RESULTS_H

  /*
  ***************************************************************
  *  Common Error codes or Status information reported by RX    *
  ***************************************************************
  */

  enum  RX_RESULTtag {
    /*  0 */ RX_OK                     = 0x0000, /* no error, valid for all functions */

    /************************************/
    /* RX Kernel functions return codes */
     /************************************/

    /*  0x1 */ RX_TIMEOUT,
    /*  0x2 */ RX_GET_NO_TOKEN,
    /*  0x3 */ RX_KNL_PRIORITY_EXIST,
    /*  0x4 */ RX_KNL_PRIORITY_INVALID,
    /*  0x5 */ RX_KNL_PRIORITY_ERROR,
    /*  0x6 */ RX_KNL_TASK_UNKNOWN,
    /*  0x7 */ RX_KNL_TASK_NOT_BLOCKED,
    /*  0x8 */ RX_KNL_STACK_UNDERRUN,
    /*  0x9 */ RX_KNL_TASK_BLOCKED,
    /*  0xA */ RX_KNL_TOKEN_INVALID,
    /*  0xB */ RX_KNL_TOKEN_DOUBLE,
    /*  0xC */ RX_KNL_TASK_DOWN,
    /*  0xD */ RX_KNL_PRIOTYPE_INVALID,
    /*  0xE */ RX_KNL_PRIOLEVEL_INVALID,
    /*  0xF */ RX_KNL_PRIORITY_RESOURCE,
    /* 0x10 */ RX_KNL_TOKEN_RESOURCE,
    /* 0x11 */ RX_KNL_KEEP_HOOK,
    /* 0x12 */ RX_KNL_TASK_REACTIVATION,
    /* 0x13 */ RX_OBJ_IN_USE,
    /* 0x14 */ RX_KNL_WRONG_PERIPHERAL_TYPE,
    /* 0x15 */ RX_KNL_TASK_NOT_DELETABLE,
    /* 0x16 */ RX_KNL_FUNCTION_NOT_AVAILABLE,
    /* 0x17 */ RX_KNL_SCHEDULER_DELAYED,
    /* 0x18 */ RX_KNL_FUNCTION_NOT_SUPPORTED,
    /* 0x19 */ RX_KNL_HANDLE_INVALID,
    /* 0x1A */ RX_KNL_OUT_OF_HANDLES,
    /* 0x1B */ RX_KNL_INVALID_CALLER,
    /* 0x1C */ RX_KNL_DUPLICATE_NAME,
    /* 0x1D */ RX_KNL_NAME_TOO_LONG,

    /* 0x100 */ RX_MEM_NO_INIT            = 0x0100,
    /* 0x101 */ RX_MEM_NAME,
    /* 0x102 */ RX_MEM_ERR,
    /* 0x103 */ RX_MEM_RESOURCE,
    /* 0x104 */ RX_MEM_ALLOCATE_HEAP_ERR,
    /* 0x105 */ RX_MEM_FREE_HEAP_ERR,
    /* 0x106 */ RX_MEM_ALLOCATE_SIZE_ERR,
    /* 0x107 */ RX_MEM_FREE_SIZE_ERR,
    /* 0x108 */ RX_MEM_MEMORY_BLOCKS,
    /* 0x109 */ RX_MEM_UNKNOWN,
    /* 0x10A */ RX_MEM_INVALID,
    /* 0x10B */ RX_MEM_NO_ACCESS,
    /* 0x10C */ RX_MEM_TIMEOUT,
    /* 0x10D */ RX_MEM_DELETED,
    /* 0x10E */ RX_MEM_INVALID_CALLER,
    /* 0x10F */ RX_MEM_NO_UPDATE,
    /* 0x110 */ RX_MEM_NO_BUFFER,
    /* 0x111 */ RX_MEM_MEMORY_NAME,
    /* 0x112 */ RX_MEM_RAM_TEST_ERR,
    /* 0x113 */ RX_MEM_SELF_TEST_ERR,
    /* 0x114 */ RX_MEM_TRIPLEBUF_DELETED,

    /* 0x200 */ RX_TIM_NO_INIT            = 0x0200,
    /* 0x201 */ RX_TIM_INVALID,
    /* 0x202 */ RX_TIM_INVALID_CALLER,
    /* 0x203 */ RX_TIM_ZERO,
    /* 0x204 */ RX_TIM_ALRDY_CLR,
    /* 0x205 */ RX_TIM_NO_EVE_HDLR,
    /* 0x206 */ RX_TIM_NOT_HALTED,
    /* 0x207 */ RX_TIM_NAME,
    /* 0x208 */ RX_TIM_CYCLE_INVALID,

    /* 0x300 */ RX_MBX_NO_INIT            = 0x0300,
    /* 0x301 */ RX_MBX_NAME,
    /* 0x302 */ RX_MBX_INVALID,
    /* 0x303 */ RX_MBX_DELETED,
    /* 0x304 */ RX_MBX_INVALID_CALLER,
    /* 0x305 */ RX_MBX_TIMEOUT,
    /* 0x306 */ RX_MBX_UNKNOWN,
    /* 0x307 */ RX_MBX_MSG_INVALID,
    /* 0x308 */ RX_MBX_SEG_INVALID,
    /* 0x309 */ RX_MBX_INTGY_LOAD,
    /* 0x30A */ RX_MBX_INTGY_DOUBLE,
    /* 0x30B */ RX_MBX_QUE_INVALID,
    /* 0x30C */ RX_MBX_QUE_EMPTY,
    /* 0x30D */ RX_MBX_QUE_INTGY_LOAD,
    /* 0x30E */ RX_MBX_ALRDY_CLR,
    /* 0x30F */ RX_MBX_NO_EVENTHDLR,
    /* 0x310 */ RX_MBX_EMPTY,
    /* 0x311 */ RX_MBX_NO_SEGMENT,
    /* 0x312 */ RX_MBX_NO_EVE_HDLR,
    /* 0x313 */ RX_MBX_TYPE_ERROR,
    /* 0x314 */ RX_MBX_PARENT_CONFLICT,
    /* 0x315 */ RX_MBX_SEG_TIMEOUT,
    /* 0x316 */ RX_MBX_NOT_INSTALLED,
    /* 0x317 */ RX_MBX_MAILBOX_LIMIT,


    /* 0x400 */ RX_SER_NO_INIT            = 0x0400,
    /* 0x401 */ RX_SER_NAME,
    /* 0x402 */ RX_SER_INVALID,
    /* 0x403 */ RX_SER_DELETED,
    /* 0x404 */ RX_SER_INVALID_CALLER,
    /* 0x405 */ RX_SER_TIMEOUT,
    /* 0x406 */ RX_SER_UNKNOWN,
    /* 0x407 */ RX_SER_RESOURCE,
    /* 0x408 */ RX_SER_OVERRUN,
    /* 0x409 */ RX_SER_NO_TASK_PENDING,
    /* 0x40A */ RX_SER_CONF_PENDING,
    /* 0x40B */ RX_SER_NO_LOAD,
    /* 0x40C */ RX_SER_ACCESS_DENIED,
    /* 0x40D */ RX_SER_FULL,
    /* 0x40E */ RX_SER_OVERRUN_WARN,

    /* 0x500 */ RX_SEM_NO_INIT            = 0x0500,
    /* 0x501 */ RX_SEM_NAME,
    /* 0x502 */ RX_SEM_INVALID,
    /* 0x503 */ RX_SEM_DELETED,
    /* 0x504 */ RX_SEM_INVALID_CALLER,
    /* 0x505 */ RX_SEM_TIMEOUT,
    /* 0x506 */ RX_SEM_UNKNOWN,
    /* 0x507 */ RX_SEM_OVERFLOW,
    /* 0x508 */ RX_SEM_NOT_AVAILABLE,
    /* 0x509 */ RX_SEM_INTGY_SEM,
    /* 0x50A */ RX_SEM_ALRDY_CLR,
    /* 0x50B */ RX_SEM_NO_COUNT,
    /* 0x50C */ RX_SEM_NO_EVE_HDLR,

    /* 0x600 */ RX_MTX_NO_INIT            = 0x0600,
    /* 0x601 */ RX_MTX_NAME,
    /* 0x602 */ RX_MTX_INVALID,
    /* 0x603 */ RX_MTX_DELETED,
    /* 0x604 */ RX_MTX_INVALID_CALLER,
    /* 0x605 */ RX_MTX_TIMEOUT,
    /* 0x606 */ RX_MTX_UNKNOWN,
    /* 0x607 */ RX_MTX_LOCKED,
    /* 0x608 */ RX_MTX_OVERFLOW,
    /* 0x609 */ RX_MTX_INTGY_MTX,
    /* 0x60A */ RX_MTX_NO_EVE_HDLR,
    /* 0x60B */ RX_MTX_ALRDY_CLR,
    /* 0x60C */ RX_MTX_INTGY_ELMT,
    /* 0x60D */ RX_MTX_NO_EVE_HANDLER,
    /* 0x60E */ RX_MTX_OUT_OF_MEMORY,
    /* 0x60F */ RX_MTX_PROCESS_INVALID,

    /* 0x700 */ RX_EVEGRP_NO_INIT         = 0x0700,
    /* 0x701 */ RX_EVEGRP_NAME,
    /* 0x702 */ RX_EVEGRP_INVALID,
    /* 0x703 */ RX_EVEGRP_DELETED,
    /* 0x704 */ RX_EVEGRP_INVALID_CALLER,
    /* 0x705 */ RX_EVEGRP_TIMEOUT,
    /* 0x706 */ RX_EVEGRP_UNKNOWN,
    /* 0x707 */ RX_EVEGRP_NOEVENT,
    /* 0x708 */ RX_EVEGRP_ALRDY_CLR,
    /* 0x709 */ RX_EVEGRP_MODE_INVALID,

    /* 0x800 */ RX_BAR_NO_INIT            = 0x0800,
    /* 0x801 */ RX_BAR_NAME,
    /* 0x802 */ RX_BAR_INVALID,
    /* 0x803 */ RX_BAR_DELETED,
    /* 0x804 */ RX_BAR_INVALID_CALLER,
    /* 0x805 */ RX_BAR_TIMEOUT,
    /* 0x806 */ RX_BAR_UNKNOWN,
    /* 0x807 */ RX_BAR_COUNT,
    /* 0x808 */ RX_BAR_NOT_REACHED,
    /* 0x809 */ RX_BAR_NO_EVE_HDLR,
    /* 0x80A */ RX_BAR_ALRDY_CLR,

    /* 0x900 */ RX_QUE_NO_INIT            = 0x0900,
    /* 0x901 */ RX_QUE_NAME,
    /* 0x902 */ RX_QUE_INVALID,
    /* 0x903 */ RX_QUE_DELETED,
    /* 0x904 */ RX_QUE_INVALID_CALLER,
    /* 0x905 */ RX_QUE_TIMEOUT,
    /* 0x906 */ RX_QUE_UNKNOWN,
    /* 0x907 */ RX_QUE_FULL,
    /* 0x908 */ RX_QUE_EMPTY,
    /* 0x909 */ RX_QUE_NO_EVE_HDLR,
    /* 0x90A */ RX_QUE_ALRDY_CLR,
    /* 0x90B */ RX_QUE_NOT_AVAILABLE,

    /* 0xB00 */ RX_TSS_NO_INIT            = 0x0B00,
    /* 0xB01 */ RX_TSS_EMPTY,
    /* 0xB02 */ RX_TSS_INVALID,
    /* 0xB03 */ RX_TSS_TASK_UNKNOWN,
    /* 0xB04 */ RX_TSS_TASK_PRIORITY,
    /* 0xB05 */ RX_TSS_TASK_NOT_LISTED,
    /* 0xB06 */ RX_TSS_PRIORITY_EXISTS,
    /* 0xB07 */ RX_TSS_TASK_CREATION,
    /* 0xB08 */ RX_TSS_TASK_REMOVE_IDLE,
    /* 0xB09 */ RX_TSS_PRIORITY_ASSIGNED,
    /* 0xB0A */ RX_TSS_USAGE_INVALID,

    /* 0xC00 */ RX_SIG_NO_INIT            = 0x0C00,
    /* 0xC01 */ RX_SIG_NAME,
    /* 0xC02 */ RX_SIG_EVENT_NOT_SET,
    /* 0xC03 */ RX_SIG_OUTOF_EVENTS,
    /* 0xC04 */ RX_SIG_TIMEOUT,
    /* 0xC05 */ RX_SIG_EVENT_INVALID,
    /* 0xC06 */ RX_SIG_EVENT_UNKNOWN,
    /* 0xC07 */ RX_SIG_MULTI_WAIT,
    /* 0xC08 */ RX_SIG_NOT_PRESENT,
    /* 0xC09 */ RX_SIG_DELETED,

    /* 0xD00 */ RX_FSM_NO_INIT            = 0x0D00,
    /* 0xD01 */ RX_FSM_NAME,
    /* 0xD02 */ RX_FSM_UNKNOWN,
    /* 0xD03 */ RX_FSM_NOT_INITIALIZED,
    /* 0xD04 */ RX_FSM_UNKNOWN_RETURN,
    /* 0xD05 */ RX_FSM_STATE_INVALID,
    /* 0xD06 */ RX_FSM_NEXTSTATE_INVALID,
    /* 0xD07 */ RX_FSM_STATEMACHINE_INVALID,
    /* 0xD08 */ RX_FSM_INVALID_CALLER,

    /* 0xE00 */ RX_CPR_NO_INIT            = 0x0E00,
    /* 0xE01 */ RX_CPR_DESTINATION_BUFFER_LEN,

    /* 0xF00 */ RX_CRC_NO_INIT            = 0x0F00,
    /* 0xF01 */ RX_CRC_NAME,
    /* 0xF02 */ RX_CRC_ORDER_INVALID,
    /* 0xF03 */ RX_CRC_POLYNOM_INVALID,
    /* 0xF04 */ RX_CRC_RESETVALUE_INVALID,
    /* 0xF05 */ RX_CRC_XORVALUE_INVALID,
    /* 0xF06 */ RX_CRC_INVALID,

    /* 0x1000 */ RX_DIA_NO_INIT            = 0x1000,
    /* 0x1001 */ RX_DIA_INFO_INVALID,
    /* 0x1002 */ RX_DIA_NO_INFO,

    /* 0x1100 */ RX_MLP_NO_INIT            = 0x1100,
    /* 0x1101 */ RX_MLP_OBJECT_UNKNOWN,
    /* 0x1102 */ RX_MLP_MASK_IN_USE,
    /* 0x1103 */ RX_MLP_INVALID_MASK,
    /* 0x1104 */ RX_MLP_NO_EVENT,
    /* 0x1105 */ RX_MLP_TIMEOUT,
    /* 0x1106 */ RX_MLP_OBJECT_DELETED,

    /* 0x1200 */ RX_FLT_NO_INIT            = 0x1200,
    /* 0x1201 */ RX_FLT_NAME,
    /* 0x1202 */ RX_FLT_NO_FAULT,

    /* 0x1300 */ RX_SYSTIME_TIMESTAMP_NS_ERROR = 0x1300,

    /***********************/
    /* Driver return codes */
    /***********************/

    /* 0x4000 */ DRV_PFLS_NO_INIT           = 0x4000,
    /* 0x4001 */ DRV_PFLS_NAME,
    /* 0x4002 */ DRV_PFLS_INVALID,
    /* 0x4003 */ DRV_PFLS_UNKNOWN,
    /* 0x4004 */ DRV_PFLS_ACCESS_INVALID,
    /* 0x4005 */ DRV_PFLS_UNKNOWN_DATATYPE,
    /* 0x4006 */ DRV_PFLS_UNKNOWN_ACCESS,
    /* 0x4007 */ DRV_PFLS_POINTER_VIOLATION,
    /* 0x4008 */ DRV_PFLS_UNKNOWN_VENDOR,
    /* 0x4009 */ DRV_PFLS_UNKNOWN_DEVICE,
    /* 0x400A */ DRV_PFLS_SECTOR_INVALID,
    /* 0x400B */ DRV_PFLS_ERASURE_FAILED,
    /* 0x400C */ DRV_PFLS_RAM_INSUFFICIENT,
    /* 0x400D */ DRV_PFLS_PROGRAM_FAILED,
    /* 0x400E */ DRV_PFLS_OFFSET_ODD,
    /* 0x400F */ DRV_PFLS_NUMBER_ODD,
    /* 0x4010 */ DRV_PFLS_RANGE_EXCEEDED,
    /* 0x4011 */ DRV_PFLS_PROTECTED,
    /* 0x4012 */ DRV_PFLS_OFFSET_EXCEEDED,
    /* 0x4013 */ DRV_PFLS_MAX_SECTOR_ENTRIES_EXCEEDED,
    /* 0x4014 */ DRV_PFLS_INIT_FUNCTION_INVALID,
    /* 0x4015 */ DRV_PFLS_SIZE_EXCEEDED,
    /* 0x4016 */ DRV_PFLS_ABORTED,

    /* 0x4080 */ DRV_CCH_NO_INIT            = 0x4080,
    /* 0x4081 */ DRV_CCH_NAME,
    /* 0x4082 */ DRV_CCH_INVALID,
    /* 0x4083 */ DRV_CCH_UNKNOWN,

    /* 0x4100 */ DRV_MMU_NO_INIT            = 0x4100,
    /* 0x4101 */ DRV_MMU_NAME,
    /* 0x4102 */ DRV_MMU_INVALID,
    /* 0x4103 */ DRV_MMU_UNKNOWN,

    /* 0x4180 */ DRV_TCM_NO_INIT            = 0x4180,
    /* 0x4181 */ DRV_TCM_NAME,
    /* 0x4182 */ DRV_TCM_INVALID,

    /* 0x4200 */ DRV_XC_NO_INIT             = 0x4200,
    /* 0x4201 */ DRV_XC_NAME,
    /* 0x4202 */ DRV_XC_UNKNOWN,
    /* 0x4203 */ DRV_XC_INVALID,
    /* 0x4204 */ DRV_XC_VERIFY_ERROR,
    /* 0x4205 */ DRV_XC_INVALID_INSTANCE,
    /* 0x4206 */ DRV_XC_IO_MEMORY_INSUFFICIENT,

    /* 0x4280 */ DRV_MPU_NO_INIT            = 0x4280,
    /* 0x4281 */ DRV_MPU_NAME,
    /* 0x4282 */ DRV_MPU_INVALID,

    /* 0x4300 */ DRV_PHY_NO_INIT            = 0x4300,
    /* 0x4301 */ DRV_PHY_NAME,
    /* 0x4302 */ DRV_PHY_UNKNOWN,
    /* 0x4303 */ DRV_PHY_INVALID,
    /* 0x4304 */ DRV_PHY_OUI_INVALID,
    /* 0x4305 */ DRV_PHY_MODEL_INVALID,
    /* 0x4306 */ DRV_PHY_REVISION_INVALID,
    /* 0x4307 */ DRV_PHY_INIT_FAILED,

    /* 0x4380 */ DRV_PIO_NO_INIT            = 0x4380,
    /* 0x4381 */ DRV_PIO_NAME,
    /* 0x4382 */ DRV_PIO_UNKNOWN,
    /* 0x4383 */ DRV_PIO_INVALID,
    /* 0x4384 */ DRV_PIO_NO_ACCESS,

    /* 0x4400 */ DRV_LED_NO_INIT            = 0x4400,
    /* 0x4401 */ DRV_LED_NAME,
    /* 0x4402 */ DRV_LED_UNKNOWN,
    /* 0x4403 */ DRV_LED_INVALID,
    /* 0x4404 */ DRV_LED_NO_ACCESS,

    /* 0x4480 */ DRV_EBUS_NO_INIT            = 0x4480,
    /* 0x4481 */ DRV_EBUS_NAME,
    /* 0x4482 */ DRV_EBUS_UNKNOWN,
    /* 0x4483 */ DRV_EBUS_INVALID,

    /* 0x4500 */ DRV_HWTIM_NO_INIT          = 0x4500,
    /* 0x4501 */ DRV_HWTIM_NAME,
    /* 0x4502 */ DRV_HWTIM_INVALID,
    /* 0x4503 */ DRV_HWTIM_NO_ACCESS,
    /* 0x4504 */ DRV_HWTIM_UNKNOWN,
    /* 0x4505 */ DRV_HWTIM_INVALID_CALLER,

    /* 0x4580 */ DRV_HIF_NO_INIT            = 0x4580,
    /* 0x4581 */ DRV_HIF_NAME,
    /* 0x4582 */ DRV_HIF_INVALID,
    /* 0x4583 */ DRV_HIF_UNKNOWN,
    /* 0x4584 */ DRV_HIF_ALREADY_INITIALIZED,
    /* 0x4585 */ DRV_HIF_BLOCK_AUTOMATICALLY_ENABLED,
    /* 0x4586 */ DRV_HIF_INVALID_INT_CONFIGURATION,
    /* 0x4587 */ DRV_HIF_MISSING_STATUS_BLOCK_ON_COM_CHANNEL,
    /* 0x4588 */ DRV_HIF_MISSING_CONTROL_BLOCK_ON_COM_CHANNEL,
    /* 0x4589 */ DRV_HIF_COS_FUNCTION_NOT_AVAIL,
    /* 0x458A */ DRV_HIF_MODE_INVALID,

    /* 0x45C0 */ DRV_HIFPIO_INIT            = 0x45C0,
    /* 0x45C1 */ DRV_HIFPIO_NAME,
    /* 0x45C2 */ DRV_HIFPIO_INVALID,
    /* 0x45C3 */ DRV_HIFPIO_UNKNOWN,

    /* 0x4600 */ DRV_INT_NO_INIT            = 0x4600,
    /* 0x4601 */ DRV_INT_NAME,
    /* 0x4602 */ DRV_INT_INVALID,
    /* 0x4603 */ DRV_INT_INVALID_CALLER,
    /* 0x4604 */ DRV_INT_UNKNOWN,
    /* 0x4605 */ DRV_INT_ALREADY_INITIALIZED,
    /* 0x4606 */ DRV_INT_STACK_SIZE_TOO_LOW,

    /* 0x4640 */ DRV_FIQ_NO_INIT            = 0x4640,
    /* 0x4641 */ DRV_FIQ_NAME,
    /* 0x4642 */ DRV_FIQ_INVALID,
    /* 0x4643 */ DRV_FIQ_INVALID_CALLER,
    /* 0x4644 */ DRV_FIQ_UNKNOWN,

    /* 0x4680 */ DRV_SFLS_NO_INIT           = 0x4680,
    /* 0x4681 */ DRV_SFLS_NAME,
    /* 0x4682 */ DRV_SFLS_INVALID,
    /* 0x4683 */ DRV_SFLS_UNKNOWN,
    /* 0x4684 */ DRV_SFLS_ACCESS_INVALID,
    /* 0x4685 */ DRV_SFLS_UNKNOWN_FLASH,
    /* 0x4686 */ DRV_SFLS_ERASURE_NOT_SUPPORTED,


    /* 0x4700 */ DRV_URT_NO_INIT            = 0x4700,
    /* 0x4701 */ DRV_URT_NAME,
    /* 0x4702 */ DRV_URT_INVALID,
    /* 0x4703 */ DRV_URT_UNKNOWN,
    /* 0x4704 */ DRV_URT_INVALID_CALLER,

    /* 0x4780 */ DRV_GPIO_NO_INIT            = 0x4780,
    /* 0x4781 */ DRV_GPIO_NAME,
    /* 0x4782 */ DRV_GPIO_INVALID,
    /* 0x4783 */ DRV_GPIO_UNKNOWN,

    /* 0x4800 */ DRV_USB_NO_INIT            = 0x4880,
    /* 0x4801 */ DRV_USB_NAME,
    /* 0x4802 */ DRV_USB_PERIPHERAL_NOT_SUPPORTED,
    /* 0x4803 */ DRV_USB_UNKNOWN,

    /* 0x4804 */ DRV_USB_OTG_NAME,      /* Instance name is to long  */
    /* 0x4805 */ DRV_USB_OTG_UNKNOWN,   /* Driver instance is unknown */
    /* 0x4806 */ DRV_USB_OTG_FATAL,     /* Serious error */

    /* 0x4807 */ DRV_USB_COM_NAME,         /* Instance name is to long  */
    /* 0x4808 */ DRV_USB_COM_UNKNOWN,      /* Driver instance is unknown */
    /* 0x4809 */ DRV_USB_COM_DISCONNECTED, /* USB was disconnected during transfer */
    /* 0x480A */ DRV_USB_COM_FATAL,        /* Serious error */
    /* 0x480B */ DRV_USB_COM_XFER_ACTIVE,  /* An read or write transfer is currently active */


    /* 0x4900 */ DRV_VOL_NO_INIT            = 0x4900,
    /* 0x4901 */ DRV_VOL_NAME,
    /* 0x4902 */ DRV_VOL_INVALID,
    /* 0x4903 */ DRV_VOL_UNKNOWN,
    /* 0x4904 */ DRV_VOL_ALREADY_MOUNTED,
    /* 0x4905 */ DRV_VOL_NOT_MOUNTED,
    /* 0x4906 */ DRV_VOL_SECTOR_INVALID,
    /* 0x4907 */ DRV_VOL_RANGE_EXCEEDED,
    /* 0x4908 */ DRV_VOL_UNKNOWN_DRIVE,
    /* 0x4909 */ DRV_VOL_IS_MOUNTED,
    /* 0x490A */ DRV_VOL_INVALID_CALLER,

    /* 0x4980 */ DRV_FTL_NO_INIT            = 0x4980,
    /* 0x4981 */ DRV_FTL_PERIPHERAL_UNKNOWN,
    /* 0x4982 */ DRV_FTL_NOT_MOUNTED,
    /* 0x4983 */ DRV_FTL_ALREADY_MOUNTED,
    /* 0x4984 */ DRV_FTL_PERIPHERAL_NOT_SUPPORTED,
    /* 0x4985 */ DRV_FTL_NO_FTL_FORMAT_FOUND,
    /* 0x4986 */ DRV_FTL_FTL_FORMAT_ERROR,
    /* 0x4987 */ DRV_FTL_FTL_SIZE_ERROR,
    /* 0x4988 */ DRV_FTL_NOT_INITIALIZED,
    /* 0x4989 */ DRV_FTL_NOT_FORMATTED,
    /* 0x498A */ DRV_FTL_RANGE_EXCEEDED,
    /* 0x498B */ DRV_FTL_NO_SPACE,
    /* 0x498C */ DRV_FTL_ERASE_ERROR,
    /* 0x498D */ DRV_FTL_NO_FREE_BLOCK,
    /* 0x498E */ DRV_FTL_NO_TRANSFER_UNIT,
    /* 0x498F */ DRV_FTL_VOLUME_TOO_BIG,
    /* 0x4990 */ DRV_FTL_FLASH_TYPE_NOT_SUPPORTED,
    /* 0x4991 */ DRV_FTL_START_OFFSET_NOT_ON_BOUNDARY,
    /* 0x4992 */ DRV_FTL_FLASH_SIZE_EXCEEDED,


    /* 0x4A00 */ DRV_EDD_NO_INIT            = 0x4a00,
    /* 0x4A01 */ DRV_EDD_NAME,
    /* 0x4A02 */ DRV_EDD_UNKNOWN,
    /* 0x4A03 */ DRV_EDD_INVALID,
    /* 0x4A04 */ DRV_EDD_NO_ACCESS,
    /* 0x4A05 */ DRV_EDD_RESOURCE,
    /* 0x4A06 */ DRV_EDD_ETH_TYPE_OCCUPIED,
    /* 0x4A07 */ DRV_EDD_NO_FRAME_BUF,
    /* 0x4A08 */ DRV_EDD_INVALID_FRAME_BUF_TYPE,
    /* 0x4A09 */ DRV_EDD_INVALID_APP_HANDLE,
    /* 0x4A0A */ DRV_EDD_PARAM_ERROR,

    /* 0x4A80 */ DRV_FIF_NO_INIT            = 0x4a80,
    /* 0x4A81 */ DRV_FIF_NAME,
    /* 0x4A82 */ DRV_FIF_UNKNOWN,
    /* 0x4A83 */ DRV_FIF_INVALID,
    /* 0x4A84 */ DRV_FIF_FIFOCHANNEL_OUTOF_RANGE,
    /* 0x4A85 */ DRV_FIF_ELEMENTDEPTH_EXCEEDED,
    /* 0x4A86 */ DRV_FIF_FIFODEPTH_INVALID,
    /* 0x4A87 */ DRV_FIF_FIFO_INVALID,
    /* 0x4A88 */ DRV_FIF_FIFO_EMPTY,
    /* 0x4A89 */ DRV_FIF_FIFO_FULL,
    /* 0x4A8A */ DRV_FIF_CHANNEL_INVALID,
    /* 0x4A8B */ DRV_FIF_FIFO_UNDERRUN,
    /* 0x4A8C */ DRV_FIF_FIFO_OVERFLOW,
    /* 0x4A8D */ DRV_FIF_FIFO_NOTEMPTY,


    /* 0x4B00 */ DRV_ICM_NO_INIT            = 0x4b00,
    /* 0x4B01 */ DRV_ICM_NAME,
    /* 0x4B02 */ DRV_ICM_UNKNOWN,
    /* 0x4B03 */ DRV_ICM_INVALID,

    /* 0x4B80 */ DRV_SECPROM_NOT_PERSONALIZED = 0x4b80,
    /* 0x4B81 */ DRV_SECPROM_NOT_INITIALIZED,
    /* 0x4B82 */ DRV_SECPROM_INVALID_INSTANCE,
    /* 0x4B83 */ DRV_SECPROM_FLUSH_FAILED,
    /* 0x4B84 */ DRV_SECPROM_EMULATED,

    /* 0x4F00 */ DRV_SPI_NO_INIT            = 0x4f00,
    /* 0x4F01 */ DRV_SPI_NAME,
    /* 0x4F02 */ DRV_SPI_INVALID,
    /* 0x4F03 */ DRV_SPI_UNKNOWN,
    /* 0x4F04 */ DRV_SPI_INVALID_CALLER,
    /* 0x4F05 */ DRV_SPI_NOT_OPEN,

    /* 0x4F80 */ DRV_IIC_NO_INIT            = 0x4f80,
    /* 0x4F81 */ DRV_IIC_NAME,
    /* 0x4F82 */ DRV_IIC_INVALID,
    /* 0x4F83 */ DRV_IIC_UNKNOWN,
    /* 0x4F84 */ DRV_IIC_INVALID_CALLER,



    /*******************************************/
    /* Hardware Abstraction Layer return codes */
    /*******************************************/

    /* 0x5000 */ HAL_URT_NO_INIT           = 0x5000,
    /* 0x5001 */ HAL_URT_NO_PORT,
    /* 0x5002 */ HAL_URT_PORT_IN_USE,
    /* 0x5003 */ HAL_URT_PORT_INVALID,
    /* 0x5004 */ HAL_URT_PARITY_INVALID,
    /* 0x5005 */ HAL_URT_STOPBIT_INVALID,
    /* 0x5006 */ HAL_URT_MODE_INVALID,
    /* 0x5007 */ HAL_URT_PORT_LIMIT,
    /* 0x5008 */ HAL_URT_PORT_NAME,
    /* 0x5009 */ HAL_URT_NO_ACCESS,
    /* 0x500A */ HAL_URT_NO_DATA,
    /* 0x500B */ HAL_URT_RECEIVE_ERROR,
    /* 0x500C */ HAL_URT_TRANSMITTER_BUSY,

    /* 0x5080 */ HAL_TCM_NO_INIT           = 0x5080,
    /* 0x5081 */ HAL_TCM_BOUNDARY,
    /* 0x5082 */ HAL_TCM_SIZE_INVALID,
    /* 0x5083 */ HAL_TCM_TYPE_INVALID,

    /* 0x5100 */ HAL_HWTIM_NO_INIT         = 0x5100,
    /* 0x5101 */ HAL_HWTIM_NAME,
    /* 0x5102 */ HAL_HWTIM_INVALID,
    /* 0x5103 */ HAL_HWTIM_NO_ACCESS,
    /* 0x5104 */ HAL_HWTIME_PRESCALE_INVALID,
    /* 0x5105 */ HAL_HWTIME_MAXVALUE_INVALID,

    /* 0x5180 */ HAL_CCH_NO_INIT           = 0x5180,
    /* 0x5181 */ HAL_CCH_UNKNOWN_TYPE,

    /* 0x5200 */ HAL_INT_NO_INIT           = 0x5200,
    /* 0x5201 */ HAL_INT_NAME,
    /* 0x5202 */ HAL_INT_NUM_INVALID,
    /* 0x5203 */ HAL_INT_TRIGGER_INVALID,
    /* 0x5204 */ HAL_INT_NO_ACCESS,
    /* 0x5205 */ HAL_INT_FULL,
    /* 0x5206 */ HAL_INT_PRIORITY_INVALID,
    /* 0x5207 */ HAL_INT_ALREADY_IN_USE,
    /* 0x5208 */ HAL_INT_DOUBLE_PRIORITY,

    /* 0x5280 */ HAL_EBUS_NO_INIT           = 0x5280,
    /* 0x5281 */ HAL_EBUS_DATAWIDTH,
    /* 0x5282 */ HAL_EBUS_NUM_INVALID,


    /* 0x5300 */ HAL_LED_NO_INIT           = 0x5300,
    /* 0x5301 */ HAL_LED_NO_ACCESS,

    /* 0x5400 */ HAL_XC_NO_INIT            = 0x5400,
    /* 0x5401 */ HAL_XC_UNKNOWN,
    /* 0x5402 */ HAL_XC_MEMTYPE_INVALID,
    /* 0x5403 */ HAL_XC_PROTECTION_FAILED,
    /* 0x5404 */ HAL_XC_BOARDERSET_FAILED,
    /* 0x5405 */ HAL_XC_BOADER_NOT_SETTABLE,

    /* 0x5500 */ HAL_PHY_NO_INIT           = 0x5500,
    /* 0x5501 */ HAL_PHY_UNKNOWN,
    /* 0x5502 */ HAL_PHY_ACCESS_INVALID,
    /* 0x5503 */ HAL_PHY_MACINSTANCE_INVALID,
    /* 0x5504 */ HAL_PHY_ACCESS_ERROR,

    /* 0x5600 */ HAL_PFL_NO_INIT           = 0x5600,
    /* 0x5601 */ HAL_PFL_UKNOWN,
    /* 0x5602 */ HAL_PFL_BUSY,
    /* 0x5603 */ HAL_PFL_ERSUSP,
    /* 0x5604 */ HAL_PFL_TIMEOUT,
    /* 0x5605 */ HAL_PFL_ERROR,
    /* 0x5606 */ HAL_PFL_PROGRAM_VOLTAGE,
    /* 0x5607 */ HAL_PFL_PROGRAM_ERROR,
    /* 0x5608 */ HAL_PFL_BLOCK_PROTECTED,
    /* 0x5609 */ HAL_PFL_CMD_SEQUENCE_ERROR,
    /* 0x560A */ HAL_PFL_WRITE_BUFFER_SETUP_FAILED,
    /* 0x560B */ HAL_PFL_PAIRED_SIZE_DIFFERS,
    /* 0x560C */ HAL_PFL_UNSUPPORTED_FLASH_TYPE,
    /* 0x560D */ HAL_PFL_PAIRED_ERASE_REGIONS_DIFFER,
    /* 0x560E */ HAL_PFL_NO_CFI_DATA,
    /* 0x560F */ HAL_PFL_UNKNOWN_COMMAND_SET,
    /* 0x5610 */ HAL_PFL_PAIRED_COMMAND_SETS_DIFFER,

    /* 0x5700 */ HAL_DPM_NO_INIT           = 0x5700,
    /* 0x5702 */ HAL_DPM_PORT_INVALID,
    /* 0x5703 */ HAL_DPM_SIZE_INVALID,
    /* 0x5704 */ HAL_DPM_HOST_MAILBOX_FULL,
    /* 0x5705 */ HAL_DPM_NO_MESSAGE,

    /* 0x5800 */ HAL_MMU_NO_INIT           = 0x5800,
    /* 0x5801 */ HAL_MMU_BOUNDARY,
    /* 0x5802 */ HAL_MMU_TYPE_UNKNOWN,

    /* 0x5880 */ HAL_MPU_NO_INIT           = 0x5880,
    /* 0x5881 */ HAL_MPU_BOUNDARY,
    /* 0x5882 */ HAL_MPU_REGION_UNKNOWN,


    /* 0x5900 */ HAL_EDD_NO_INIT           = 0x5900,
    /* 0x5901 */ HAL_EDD_UNKNOWN,
    /* 0x5902 */ HAL_EDD_ACCESS_INVALID,
    /* 0x5903 */ HAL_EDD_ACCESS_ERROR,
    /* 0x5904 */ HAL_EDD_HANDLE_INVALID,
    /* 0x5905 */ HAL_EDD_CALLBACK_INVALID,
    /* 0x5906 */ HAL_EDD_BUFFER_INVALID,
    /* 0x5907 */ HAL_EDD_NO_RESOURCE,
    /* 0x5908 */ HAL_EDD_MGMT_FRAME,         /* Was not a real ethernet frame in hal */
    /* 0x5909 */ HAL_EDD_FRAME_DEQUEUED,     /* Frame was taken out by Hal */
    /* 0x590A */ HAL_EDD_TX_LATE_COLLISION,
    /* 0x590B */ HAL_EDD_TX_LINK_DOWN,
    /* 0x590C */ HAL_EDD_TX_EXCESSIVE_COLLISION,
    /* 0x590D */ HAL_EDD_TX_FRAME_LENGTH_ERROR,
    /* 0x590E */ HAL_EDD_TX_POINTER_ADDRESS_ERROR,
    /* 0x590F */ HAL_EDD_TX_FIFO_UNDERFLOW,
    /* 0x5910 */ HAL_EDD_RX_FCS_ERROR,
    /* 0x5911 */ HAL_EDD_RX_ALIGNMENT_ERROR,
    /* 0x5912 */ HAL_EDD_RX_FRAME_TOO_LONG,
    /* 0x5913 */ HAL_EDD_RX_RUNT_FRAME,
    /* 0x5914 */ HAL_EDD_RX_COLLISION_FRAGMENT,
    /* 0x5915 */ HAL_EDD_RX_START_OF_FRAME_ERROR,
    /* 0x5916 */ HAL_EDD_RX_FIFO_OVERFLOW,
    /* 0x5917 */ HAL_EDD_PARAM_ERROR,
    /* 0x5918 */ HAL_EDD_COMMAND_INVALID,
    /* 0x5919 */ HAL_EDD_INTERRUPT_CONFIG_INVALID,

    /* 0x5A00 */ HAL_ICM_NO_INIT           = 0x5a00,
    /* 0x5A01 */ HAL_ICM_UNKNOWN,
    /* 0x5A02 */ HAL_ICM_ACCESS_INVALID,
    /* 0x5A03 */ HAL_ICM_ACCESS_ERROR,
    /* 0x5A04 */ HAL_ICM_HANDLE_INVALID,

    /* 0x5B80 */ HAL_HIF_NO_INIT           = 0x5b80,
    /* 0x5B81 */ HAL_HIF_ILLVECTID,
    /* 0x5B82 */ HAL_HIF_NOHANDLER,
    /* 0x5B83 */ HAL_HIF_ILLSUBIRQMASK,
    /* 0x5B84 */ HAL_HIF_UNINITIALIZED,
    /* 0x5B85 */ HAL_HIF_ALREADY_INITIALIZED,
    /* 0x5B86 */ HAL_HIF_CHANNEL_NUM_OF_CHANNELBLOCKS_EXCEEDED,
    /* 0x5B87 */ HAL_HIF_CHANNEL_SIZE_INVALID,
    /* 0x5B88 */ HAL_HIF_CHANNEL_LAYOUT_UNKNOWN,
    /* 0x5B89 */ HAL_HIF_CHANNEL_SIZE_EXCEEDED,
    /* 0x5B8A */ HAL_HIF_CHANNEL_HOST_MAILBOX_FULL,
    /* 0x5B8B */ HAL_HIF_CHANNEL_SEGMENT_LIMIT,
    /* 0x5B8C */ HAL_HIF_CHANNEL_SEGMENT_UNUSED,
    /* 0x5B8D */ HAL_HIF_CHANNEL_NAME_INVALID,
    /* 0x5B8E */ HAL_HIF_CHANNEL_UNKNOWN,
    /* 0x5B8F */ HAL_HIF_CHANNEL_ALREADY_INITIALIZED,
    /* 0x5B90 */ HAL_HIF_BLOCK_UNKNOWN,
    /* 0x5B91 */ HAL_HIF_BLOCK_CREATION_FAILED,
    /* 0x5B92 */ HAL_HIF_BLOCK_OFFSET_INVALID,
    /* 0x5B93 */ HAL_HIF_FUNCTION_POSITION_INVALID,
    /* 0x5B94 */ HAL_HIF_FUNCTION_ALREADY_INITIALIZED,
    /* 0x5B95 */ HAL_HIF_BLOCK_SIZE_EXCEEDED,
    /* 0x5B96 */ HAL_HIF_MEM_RESOURCE,
    /* 0x5B97 */ HAL_HIF_INVALID_SIZE,
    /* 0x5B98 */ HAL_HIF_INVALID_PHYS_MEMORY_BASE,
    /* 0x5B99 */ HAL_HIF_INVALID_PHYS_MEMORY_SIZE,

    /* 0x5D00 */ HAL_SPI_NO_INIT           = 0x5d00,
    /* 0x5D01 */ HAL_SPI_NULL,
    /* 0x5D02 */ HAL_SPI_PORT_INVALID,
    /* 0x5D03 */ HAL_SPI_SLAVE_INVALID,
    /* 0x5D04 */ HAL_SPI_BUSY,
    /* 0x5D05 */ HAL_WAIT_FOR_SIGNAL,

    /* 0x5E00 */ HAL_IIC_NO_INIT           = 0x5e00,
    /* 0x5E01 */ HAL_IIC_NULL,
    /* 0x5E02 */ HAL_IIC_NOT_OPEN,
    /* 0x5E03 */ HAL_IIC_PORT_INVALID,
    /* 0x5E04 */ HAL_IIC_SLAVE_INVALID,
    /* 0x5E05 */ HAL_IIC_TIMEOUT,




    /***************************/
    /* Middleware return codes */
    /***************************/

    /* 0x6000 */ MID_NVR_NO_INIT            = 0x6000,
    /* 0x6001 */ MID_NVR_PORT_LIMIT,

    /* 0x6080 */ MID_COM_NO_INIT            = 0x6080,
    /* 0x6081 */ MID_COM_TASK_FAILED,

    /* 0x6100 */ MID_STA_NO_INIT            = 0x6100,
    /* 0x6101 */ MID_STA_TASK_FAILED,

    /* 0x6180 */ MID_FAT_NO_INIT            = 0x6180,
    /* 0x6181 */ MID_FAT_NOT_FOUND,
    /* 0x6182 */ MID_FAT_VOLUME_INVALID,
    /* 0x6183 */ MID_FAT_FATTYPE_UNKNOWN,
    /* 0x6184 */ MID_FAT_VOLUME_FULL,
    /* 0x6185 */ MID_FAT_FILE_ALREADY_EXISTS,
    /* 0x6186 */ MID_FAT_FILE_NOT_FOUND,
    /* 0x6187 */ MID_FAT_FILE_INVALID,
    /* 0x6188 */ MID_FAT_ORIGIN_INVALID,
    /* 0x6189 */ MID_FAT_PATH_NOT_EXISTS,
    /* 0x618A */ MID_FAT_PATH_ATTRIBUTE,
    /* 0x618B */ MID_FAT_DIRECTORY_NOT_EMPTY,
    /* 0x618C */ MID_FAT_END_OF_FILE,
    /* 0x618D */ MID_FAT_DIRECTORY_ALREADY_EXISTS,
    /* 0x618E */ MID_FAT_NO_FILESYSTEM_INSTALLED,
    /* 0x618F */ MID_FAT_VOLUME_NOT_MOUNTED,
    /* 0x6190 */ MID_FAT_FILE_IS_DIRECTORY,
    /* 0x6191 */ MID_FAT_FILE_READONLY,
    /* 0x6192 */ MID_FAT_FILE_OPEN,
    /* 0x6193 */ MID_FAT_FATTABLE_CORRUPT,
    /* 0x6194 */ MID_FAT_NO_MORE_RECORD,
    /* 0x6195 */ MID_FAT_READ_ERROR,
    /* 0x6196 */ MID_FAT_WRITE_ERROR,
    /* 0x6197 */ MID_FAT_CLUSTER_ALLOC_ERROR,
    /* 0x6198 */ MID_FAT_CLUSTER_FAT_ERROR,
    /* 0x6199 */ MID_FAT_CLUSTER_CHANGE_ERROR,
    /* 0x619A */ MID_FAT_READ_FAT_ERROR,
    /* 0x619B */ MID_FAT_WRITE_FAT_ERROR,
    /* 0x619C */ MID_FAT_WRITE_DIR_ERROR,
    /* 0x619D */ MID_FAT_READ_DIR_ERROR,
    /* 0x619E */ MID_FAT_READ_FSINFO_ERROR,
    /* 0x619F */ MID_FAT_WRITE_FSINFO_ERROR,
    /* 0x61A0 */ MID_FAT_FORMAT_NOT_ALLOWED,
    /* 0x61A1 */ MID_FAT_IS_NOT_DIRECTORY,
    /* 0x61A2 */ MID_FAT_DIR_HANDLE_INVALID,
    /* 0x61A3 */ MID_FAT_PATH_INVALID,

    /* 0x6200 */ MID_DBM_NO_INIT            = 0x6200,
    /* 0x6201 */ MID_DBM_NO_SYSVOLUME,
    /* 0x6202 */ MID_DBM_FILE_INVALID,
    /* 0x6203 */ MID_DBM_TABLE_UNKNOWN,
    /* 0x6204 */ MID_DBM_HANDLE_INVALID,
    /* 0x6205 */ MID_DBM_DATASET_ERROR,
    /* 0x6206 */ MID_DBM_INDEX_INVALID,
    /* 0x6207 */ MID_DBM_UNKNOWN_FILE,
    /* 0x6208 */ MID_DBM_FNC_ONLY_ON_NEW_DBM,
    /* 0x6209 */ MID_DBM_NO_REC0,
    /* 0x620A */ MID_DBM_FNC_NEEDS_LL_TABLE,
    /* 0x620B */ MID_DBM_INVALID_PARAM,
    /* 0x620C */ MID_DBM_KEY_INVALID,
    /* 0x620D */ MID_DBM_KEY_NOT_FOUND,
    /* 0x620E */ MID_DBM_NO_RECORD_AVAILABLE,
    /* 0x620F */ MID_DBM_FNC_NOT_ON_LL,
    /* 0x6210 */ MID_DBM_UNKNOWN_PROPERTY,
    /* 0x6211 */ MID_DBM_FNC_NOT_ON_TAB_TYPE1,
    /* 0x6212 */ MID_DBM_CHECKSUM_INVALID,
    /* 0x6213 */ MID_DBM_BOUNDARY_INVALID,
    /* 0x6214 */ MID_DBM_NAME,
    /* 0x6215 */ MID_DBM_FILE_CORRUPT,
    /* 0x6216 */ MID_DBM_FILE_UNSUPPORTED_VERSION,
    /* 0x6217 */ MID_DBM_LAST_REACHED,
    /* 0x6218 */ MID_DBM_NOT_IMPLEMENTED,

    /* 0x6280 */ MID_LED_NO_INIT            = 0x6280,

    /* 0x6300 */ MID_SYS_NO_INIT            = 0x6300,
    /* 0x6301 */ MID_SYS_NO_LINKAGE         = 0x6301,

    /**************************************************/
    /* Application Abstraction Interface return codes */
    /**************************************************/


    /* Software Interrupt Module */

    /* 0 */  AAI_OK                     = 0x7FFF


  };
  typedef UINT32 RX_RESULT;
#endif
