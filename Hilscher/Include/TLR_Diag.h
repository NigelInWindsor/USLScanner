/******************************************************************************

  Copyright (C) 2010 [Hilscher Gesellschaft für Systemautomation mbH]

  This program can be used by everyone according to the netX Open Source
  Software license. The license agreement can be downloaded from
  http://www.industrialNETworX.com

*******************************************************************************

  Last Modification:
    @version $Id: TLR_Diag.h 1378 2010-04-30 13:26:25Z Robert $

  Description:
    TLR diagnostics definitions

  Changes:
    Date        Author        Description
  ---------------------------------------------------------------------------
    2010-04-14  NC            Added CONFIGURATION_RELOAD_REQ/_CNF
  
    2010-03-23  NC            File created.

******************************************************************************/


#ifndef __TLR_DIAG_H
#define __TLR_DIAG_H

#ifdef __cplusplus
  extern "C" {
#endif  /* __cplusplus */



/*****************************************************************************/
/* Include Files Required                                                    */
/*****************************************************************************/


#include "TLR_Types.h"



/*****************************************************************************/
/* Symbol Definitions                                                        */
/*****************************************************************************/


/*********************** Common Diagnostics Commands *************************/

#define DIAG_INFO_GET_COMMON_STATE_REQ              0x00002F00
#define DIAG_INFO_GET_COMMON_STATE_CNF              0x00002F01

#define DIAG_INFO_GET_WATCHDOG_TIME_REQ             0x00002F02
#define DIAG_INFO_GET_WATCHDOG_TIME_CNF             0x00002F03

#define DIAG_INFO_SET_WATCHDOG_TIME_REQ             0x00002F04
#define DIAG_INFO_SET_WATCHDOG_TIME_CNF             0x00002F05

#define DIAG_INFO_GET_EXTENDED_STATE_REQ            0x00002F06
#define DIAG_INFO_GET_EXTENDED_STATE_CNF            0x00002F07

#define CONFIGURATION_RELOAD_REQ                    0x00002F80
#define CONFIGURATION_RELOAD_CNF                    0x00002F81


/********************** Common Task Diagnostics Codes ************************/

#define TLR_DIAG_STA_OK                             (0x00000000)  /* Task status OK. */
#define TLR_DIAG_STA_DISABLED                       (0x00000001)  /* Task currently disabled. */
#define TLR_DIAG_STA_IDLE                           (0x00000002)  /* Task not communicating. */
#define TLR_DIAG_STA_INIT_LOCAL_FAILED              (0xC0000003)  /* Error during initialization of local task resources. */
#define TLR_DIAG_STA_INIT_REMOTE_FAILED             (0xC0000004)  /* Error during initialization of remote task resources. */
#define TLR_DIAG_STA_SELF_IDENTIFY_FAILED           (0xC0000005)  /* Task self-Identification failed. */
#define TLR_DIAG_STA_PROCESS_QUEUE_CREATE_FAILED    (0xC0000006)  /* Creation of process queue failed. */
#define TLR_DIAG_STA_PACKET_POOL_CREATE_FAILED      (0xC0000007)  /* Creation of packet pool failed. */
#define TLR_DIAG_STA_INFO_FIELD_CREATE_FAILED       (0xC0000008)  /* Creation of diag information field failed. */
#define TLR_DIAG_STA_INFO_FIELD_ZERO_CREATE_FAILED  (0xC0000009)  /* Creation of basic diag information field failed. */
#define TLR_DIAG_STA_OUTOFMEMORY                    (0xC000000A)  /* Out of memory while creating task resources. */
#define TLR_DIAG_STA_STARTUP_PARAMETER              (0xC000000B)  /* Task startup parameters not defined. */
#define TLR_DIAG_STA_WATCHDOG                       (0xC000000C)  /* Watchdog error. */
#define TLR_DIAG_STA_RESET                          (0x4000000D)  /* Reset in progress. */
#define TLR_DIAG_STA_CONFIGURATION                  (0x4000000E)  /* No configuration active. */
#define TLR_DIAG_STA_FATAL_ERROR                    (0xC000000F)  /* Fatal error. */



/*****************************************************************************/
/* Class, Type, and Structure Definitions                                    */
/*****************************************************************************/


/************************ Common Diagnostics Packets *************************/

/* Get watchdog timeout */
typedef TLR_EMPTY_PACKET_T          DIAG_INFO_GET_WATCHDOG_TIME_REQ_T;
typedef struct DIAG_INFO_GET_WATCHDOG_TIME_CNF_DATA_Ttag
{
  /** watchdog time in us */
  TLR_UINT32 ulWdgTime;
} DIAG_INFO_GET_WATCHDOG_TIME_CNF_DATA_T;

typedef struct
{
  TLR_PACKET_HEADER_T                     tHead;
  DIAG_INFO_GET_WATCHDOG_TIME_CNF_DATA_T  tData;
} DIAG_INFO_GET_WATCHDOG_TIME_CNF_T;


/* Set watchdog timeout */
typedef struct
{
  /** watchdog time in us */
  TLR_UINT32 ulWdgTime;
} DIAG_INFO_SET_WATCHDOG_TIME_REQ_DATA_T;

typedef struct
{
  TLR_PACKET_HEADER_T                     tHead;
  DIAG_INFO_SET_WATCHDOG_TIME_REQ_DATA_T  tData;
} DIAG_INFO_SET_WATCHDOG_TIME_REQ_T;

typedef TLR_EMPTY_PACKET_T          DIAG_INFO_SET_WATCHDOG_TIME_CNF_T;


typedef TLR_EMPTY_PACKET_T          CONFIGURATION_RELOAD_REQ_T;
typedef TLR_EMPTY_PACKET_T          CONFIGURATION_RELOAD_CNF_T;


/* Get common status block */
typedef struct
{
  TLR_UINT8 abCommonState[64];
} DIAG_INFO_GET_COMMON_STATE_CNF_DATA_T;

typedef struct
{
  TLR_PACKET_HEADER_T                     tHead;
} DIAG_INFO_GET_COMMON_STATE_REQ_T;

typedef struct
{
  TLR_PACKET_HEADER_T                     tHead;
  DIAG_INFO_GET_COMMON_STATE_CNF_DATA_T   tData;
} DIAG_INFO_GET_COMMON_STATE_CNF_T;


/* Get extended status block */
typedef struct
{
  TLR_UINT32 ulOffset;
  TLR_UINT32 ulDataLen;
} DIAG_INFO_GET_EXTENDED_STATE_REQ_DATA_T;

typedef struct
{
  TLR_PACKET_HEADER_T                     tHead;
  DIAG_INFO_GET_EXTENDED_STATE_REQ_DATA_T tData;
} DIAG_INFO_GET_EXTENDED_STATE_REQ_T;

typedef struct
{
  TLR_UINT32 ulOffset;
  TLR_UINT32 ulDataLen;
  TLR_UINT8  abExtendedState[432];
} DIAG_INFO_GET_EXTENDED_STATE_CNF_DATA_T;

typedef struct
{
  TLR_PACKET_HEADER_T                     tHead;
  DIAG_INFO_GET_EXTENDED_STATE_CNF_DATA_T tData;
} DIAG_INFO_GET_EXTENDED_STATE_CNF_T;


/*********************** Task Diagnostics Information ************************/
typedef struct
{
  TLR_UINT32        ulTskIdentifier;        /* task identifier see TLR_TaskIdentifier.h */
  TLR_UINT16        usTskMajorVersion;      /* major number of the task (or stack) version */
  TLR_UINT16        usTskMinorVersion;      /* minor number of the task (or stack) version */
  TLR_UINT32        ulMaximumPacketSize;    /* maximum packet size used by the task */
  TLR_UINT32        ulDefaultQue;           /* handle of the task's default message queue */
  TLR_TASK_UID_T    tTaskUid;               /* UUID of the task */
  TLR_RESULT        eInitRslt;              /* result code from self-initialization */
  TLR_UINT32        pvRsc;                  /* address of the task's resource structure */
} TLR_DIA_TASK_INFO_T;



/*****************************************************************************/
/* Global Variables                                                          */
/*****************************************************************************/


/* none */



/*****************************************************************************/
/* Macros                                                                    */
/*****************************************************************************/


/* none */



/*****************************************************************************/
/* Functions                                                                 */
/*****************************************************************************/


/* none */



#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __TLR_DIAG_H */
