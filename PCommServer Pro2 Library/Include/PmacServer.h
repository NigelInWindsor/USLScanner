

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Tue Feb 21 10:12:07 2006
 */
/* Compiler settings for .\PmacServer.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __PmacServer_h__
#define __PmacServer_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IPmacDevice_FWD_DEFINED__
#define __IPmacDevice_FWD_DEFINED__
typedef interface IPmacDevice IPmacDevice;
#endif 	/* __IPmacDevice_FWD_DEFINED__ */


#ifndef ___IPmacDeviceEvents_FWD_DEFINED__
#define ___IPmacDeviceEvents_FWD_DEFINED__
typedef interface _IPmacDeviceEvents _IPmacDeviceEvents;
#endif 	/* ___IPmacDeviceEvents_FWD_DEFINED__ */


#ifndef __PmacDevice_FWD_DEFINED__
#define __PmacDevice_FWD_DEFINED__

#ifdef __cplusplus
typedef class PmacDevice PmacDevice;
#else
typedef struct PmacDevice PmacDevice;
#endif /* __cplusplus */

#endif 	/* __PmacDevice_FWD_DEFINED__ */


#ifndef __IPmacNC_FWD_DEFINED__
#define __IPmacNC_FWD_DEFINED__
typedef interface IPmacNC IPmacNC;
#endif 	/* __IPmacNC_FWD_DEFINED__ */


#ifndef ___IPmacNCEvents_FWD_DEFINED__
#define ___IPmacNCEvents_FWD_DEFINED__
typedef interface _IPmacNCEvents _IPmacNCEvents;
#endif 	/* ___IPmacNCEvents_FWD_DEFINED__ */


#ifndef __PmacNC_FWD_DEFINED__
#define __PmacNC_FWD_DEFINED__

#ifdef __cplusplus
typedef class PmacNC PmacNC;
#else
typedef struct PmacNC PmacNC;
#endif /* __cplusplus */

#endif 	/* __PmacNC_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

/* interface __MIDL_itf_PmacServer_0000 */
/* [local] */ 

#define	MAXBROTBUFFERS	( 8 )

#define	MAXDEVICES	( 8 )

#define	MAXMOTORS	( 8 )

#define	MAXCOORDS	( 8 )

#define	MAXMOTORSTURBO	( 32 )

#define	MAXCOORDSTURBO	( 16 )

#define	MAXNCCONTROLS	( 2 )

#define	MAXGATHERS	( 24 )

#define	MAXGATHERS2	( 48 )

#define	MAXRTGATHERS	( 3 )

#define	MAX_DPRBUFFER	( 160 )

#define	MAX_DPRREADBUFFER	( 256 )

#define	DPR_READ_COPY_BUFFER_SIZE	( 260 )

#define	MAXDATABUFFERS	( 2 )

#define MST_MOTOR_ACTIVE				& 0x800000000000
#define MST_NEG_END_LIMIT_SET			& 0x400000000000
#define MST_POS_END_LIMT_SET			& 0x200000000000
#define MST_EXT_SERVO_ALGO_ENA			& 0x100000000000
#define MST_AMPLIFIER_ENABLE			& 0x080000000000
#define MST_OPEN_LOOP_MODE				& Ox040000000000
#define MST_MOVE_TIME_ACTIVE			& 0x020000000000
#define MST_INTEGRATE_MODE				& 0x010000000000
#define MST_DWELL_IN_PROGRESS			& 0x008000000000
#define MST_DATA_BLOCK_ERROR			& 0x004000000000
#define MST_DESIRED_VELOCITY_0			& 0x002000000000
#define MST_ABORT_DECELERATE_PROGS		& 0x001000000000
#define MST_BLOCK_REQUEST				& 0x000800000000
#define MST_HOME_SEARCH_PROGS			& 0x000400000000
#define MST_USER_WRITTEN_PHASE			& 0x000200000000
#define MST_USER_WRITTEN_SERVO			& 0x000100000000
#define MST_Y_ADDRS_COMMUTE				& 0x000080000000
#define MST_COMMUTATION_ENABLE			& 0x000040000000
#define MST_POS_FOLLOW_OFFSET_MODE		& 0x000020000000
#define MST_POS_FOLLOW_ENABLE			& 0x000010000000
#define MST_CAPTURE_ERROR_ENABLE		& 0x000008000000
#define MST_SOFTWARE_CAPT_ENABLE		& 0x000004000000
#define MST_SIGN_MAGNITUDE_SERVO		& 0x000002000000
#define MST_RAPID_MAX_VELOCITY			& 0x000001000000
#define MST_CS_1_BIT_3					& 0x000000800000
#define MST_CS_1_BIT_2					& 0x000000400000
#define MST_CS_1_BIT_1					& 0x000000200000
#define MST_CS_1_BIT_0					& 0x000000100000
#define MST_CS_AXIS_DEF_BIT_3			& 0x000000080000
#define MST_CS_AXIS_DEF_BIT_2			& 0x000000040000
#define MST_CS_AXIS_DEF_BIT_1			& 0x000000020000
#define MST_CS_AXIS_DEF_BIT_0			& 0x000000010000
#define MST_ASSIGNED_TO_CS				& 0x000000008000
#define MST_RESERVER_FOR_FUTURE			& 0x000000004000
#define MST_FOREGROUND_IN_POSITION		& 0x000000002000
#define MST_DESIRED_POSITION_STOP		& 0x000000001000
#define MST_STOP_ON_POSITION_LIMIT		& 0x000000000800
#define MST_HOME_COMPLETE				& 0x000000000400
#define MST_MOTOR_PHASE_REQUEST			& 0x000000000200
#define MST_PHASING_SEARCH_ERROR		& 0x000000000100
#define MST_TIGGER_MOVE					& 0x000000000080
#define MST_INTEG_FATAL_FOLLOW_ERR		& 0x000000000040
#define MST_I2T_AMP_FAULT_ERROR			& 0x000000000020
#define MST_BACKLASH_DIRECTION_FLAG		& 0x000000000010
#define MST_AMP_FAULT_ERROR				& 0x000000000008
#define MST_FATAL_ERROR_EXCEEDED		& 0x000000000004
#define MST_WARNING_ERROR_EXCEEDED		& 0x000000000002
#define MST_IN_POSITION_TRUE			& 0x000000000001
#define MSNT_MOTOR_ACTIVE				& 0x800000000000
#define MSNT_NEG_END_LIMIT_SET			& 0x400000000000
#define MSNT_POS_END_LIMT_SET			& 0x200000000000
#define MSNT_HANDWHEEL_ENABLE			& 0x100000000000
#define MSNT_PHASED_MOTOR				& 0x080000000000
#define MSNT_OPEN_LOOP_MODE				& Ox040000000000
#define MSNT_RUNNING_A_PROGRAM			& 0x020000000000
#define MSNT_INTEGRATE_MODE				& 0x010000000000
#define MSNT_DWELL_IN_PROGRESS			& 0x008000000000
#define MSNT_DATA_BLOCK_ERROR			& 0x004000000000
#define MSNT_DESIRED_VELOCITY_0			& 0x002000000000
#define MSNT_ABORT_DECELERATION			& 0x001000000000
#define MSNT_BLOCK_REQUEST				& 0x000800000000
#define MSNT_HOME_SEARCH_ACTIVE			& 0x000400000000
#define MSNT_INTERNAL_X9				& 0x000200000000
#define MSNT_INTERNAL_X8				& 0x000100000000
#define MSNT_INTERNAL_X7				& 0x000080000000
#define MSNT_INTERNAL_X6				& 0x000040000000
#define MSNT_INTERNAL_X5				& 0x000020000000
#define MSNT_INTERNAL_X4				& 0x000010000000
#define MSNT_INTERNAL_X3				& 0x000008000000
#define MSNT_INTERNAL_X2				& 0x000004000000
#define MSNT_INTERNAL_X1				& 0x000002000000
#define MSNT_INTERNAL_X0				& 0x000001000000
#define MSNT_ASSIGNED_TO_CS				& 0x000000800000
#define MSNT_CS_1_BIT_2					& 0x000000400000
#define MSNT_CS_1_BIT_1					& 0x000000200000
#define MSNT_CS_1_BIT_0					& 0x000000100000
#define MSNT_RESERVED_Y19				& 0x000000080000
#define MSNT_RESERVED_Y18				& 0x000000040000
#define MSNT_RESERVED_Y17				& 0x000000020000
#define MSNT_RESERVED_Y16				& 0x000000010000
#define MSNT_RESERVED_Y15				& 0x000000008000
#define MSNT_AMPLIFIER_ENABLED			& 0x000000004000
#define MSNT_RESERVED_Y13				& 0x000000002000
#define MSNT_RESERVED_Y12				& 0x000000001000
#define MSNT_STOP_ON_POSITION_LIMIT		& 0x000000000800
#define MSNT_HOME_COMPLETE				& 0x000000000400
#define MSNT_RESERVED_Y9				& 0x000000000200
#define MSNT_PHASING_SEARCH_ERROR		& 0x000000000100
#define MSNT_TIGGER_MOVE				& 0x000000000080
#define MSNT_INTEG_FATAL_FOLLOW_ERR		& 0x000000000040
#define MSNT_I2T_AMP_FAULT_ERROR		& 0x000000000020
#define MSNT_BACKLASH_DIRECTION_FLAG	& 0x000000000010
#define MSNT_AMP_FAULT_ERROR			& 0x000000000008
#define MSNT_FATAL_ERROR_EXCEEDED		& 0x000000000004
#define MSNT_WARNING_ERROR_EXCEEDED		& 0x000000000002
#define MSNT_IN_POSITION_TRUE			& 0x000000000001
#define GSNT_REAL_TIME_INTERR_ACTIVE	& 0x800000000000
#define GSNT_REAL_TIME_INTERR_REENTRY	& 0x400000000000
#define	GSNT_SERVO_ACTIVE				& 0x200000000000
#define GSNT_SERVO_ERROR				& 0x100000000000
#define GSNT_DATA_GATHER_ENABLED		& 0x080000000000
#define GSNT_GATHER_ON_NEXT_SERVO		& 0x040000000000
#define GSNT_GATHER_ON_EXTERNAL_TRIG	& 0x020000000000
#define GSNT_RESERVED_X16				& 0x010000000000
#define GSNT_RESERVED_X15				& 0x008000000000
#define GSNT_COMPENSATE_TABLE_OPEN		& 0x004000000000
#define GSNT_GENERAL_CHECKSUM_ERR		& 0x002000000000
#define GSNT_FIRMWARE_CHECKSUM_ERR		& 0x001000000000
#define GSNT_DPRAM_ERROR				& 0x000800000000
#define GSNT_EAROM_ERROR				& 0x000400000000
#define GSNT_INTERNAL_X9				& 0x000200000000
#define GSNT_INTERNAL_X8				& 0x000100000000
#define GSNT_INTERNAL_X7				& 0x000080000000
#define GSNT_TWS_VAR_PARITY_ERROR		& 0x000040000000
#define GSNT_MARCO_AUX_COMM_ERROR		& 0x000020000000
#define GSNT_RESERVED_X4				& 0x000010000000
#define GSNT_RESERVED_X3				& 0x000008000000
#define GSNT_RESERVED_X2				& 0x000004000000
#define GSNT_ALL_CARDS_ADD_SERIALLY		& 0x000002000000
#define GSNT_THIS_CARDS_ADD_SERIALLY	& 0x000001000000
#define GSNT_EXTENDED_READ				& 0x000000800000
#define GSNT_HOST_PORT_COMM_MODE		& 0x000000400000
#define GSNT_INTERNAL_Y21				& 0x000000200000
#define GSNT_INTERNAL_Y20				& 0x000000100000
#define GSNT_MOTION_BUFFER_OPEN			& 0x000000080000
#define GSNT_ROTARY_BUFFER_OPEN			& 0x000000040000
#define GSNT_PLC_BUFFER_OPEN			& 0x000000020000
#define GSNT_PLC_COMMAND				& 0x000000010000
#define	GSNT_VME_PORT_COMM_MODE			& 0x000000008000
#define	GSNT_INTERNAL_Y14				& 0x000000004000
#define GSNT_INTERNAL_Y13				& 0x000000002000
#define	GSNT_INTERNAL_Y12				& 0x000000001000
#define GSNT_FIXED_BUFFER_FULL			& 0x000000000800
#define GSNT_INTERNAL_Y10				& 0x000000000400
#define GSNT_INTERNAL_Y9				& 0x000000000200
#define GSNT_INTERNAL_Y8				& 0x000000000100
#define GSNT_RESERVED_Y7				& 0x000000000080
#define GSNT_RESERVED_Y6				& 0x000000000040
#define GSNT_RESERVED_Y5				& 0x000000000020
#define GSNT_RESERVED_Y4				& 0x000000000010
#define GSNT_RESERVED_Y3				& 0x000000000008
#define GSNT_RESERVED_Y2				& 0x000000000004
#define	GSNT_RESERVED_Y1				& 0x000000000002
#define GSNT_RESERVED_Y0				& 0x000000000001
#define GST_MAIN_ERROR					& 0x800000000000
#define GST_RTI_REENTRY_ERROR			& 0x400000000000
#define	GST_CPU_TYPE_1					& 0x200000000000
#define GST_SERVO_ERORR					& 0x100000000000
#define GST_DATA_GATH_ENABLED			& 0x080000000000
#define GST_RESERVED_X18				& 0x040000000000
#define GST_GATHER_EXTERNAL_TRIG		& 0x020000000000
#define GST_SMALL_MEM_TURBO_PMAC		& 0x010000000000
#define GST_INTERNAL_15					& 0x008000000000
#define GST_COMPENSATE_TABLE_ON			& 0x004000000000
#define GST_GENERAL_CHECKSUM_ERR		& 0x002000000000
#define GST_FIRMWARE_CHECKSUM_ERR		& 0x001000000000
#define GST_DPRAM_ERROR					& 0x000800000000
#define GST_EAROM_ERROR					& 0x000400000000
#define GST_REAL_TIME_INTERR_WARN		& 0x000200000000
#define GST_ILLEGAL_L_VAR_DEF			& 0x000100000000
#define GST_SERVO_MACRO_IC_CONFIG_ERR	& 0x000080000000
#define GST_TWS_VAR_PARTITY_ERROR		& 0x000040000000
#define GST_MACRO_COMM_ERROR			& 0x000020000000
#define GST_MACRO_RING_ERROR			& 0x000010000000
#define GST_NO_PHASE_CLOCK_ERROR		& 0x000008000000
#define GST_RESERVED_X2					& 0x000004000000
#define GST_ALL_CARDS_ADD_SERIALLY		& 0x000002000000
#define GST_THIS_CARDS_ADD_SERIALLY		& 0x000001000000
#define GST_TURBO_ULTRALITE				& 0x000000800000
#define GST_TURBO_VME					& 0x000000400000
#define GST_CPU_TYPE					& 0x000000200000
#define GST_BINARY_ROTARY_BUFF_OPEN		& 0x000000100000
#define GST_MOTION_BUFFER_OPEN			& 0x000000080000
#define GST_ASCII_ROTARY_BUFFER_OPEN	& 0x000000040000
#define GST_PLC_BUFFER_OPEN				& 0x000000020000
#define GST_UMAC_TURBO					& 0x000000010000
#define	GST_INTERNAL_Y15				& 0x000000008000
#define	GST_INTERNAL_Y14				& 0x000000004000
#define GST_RESERVED_Y13				& 0x000000002000
#define	GST_RESERVED_Y12				& 0x000000001000
#define GST_FIXED_BUFFER_FULL			& 0x000000000800
#define GST_MACRO_RING_TEST_ENABLE		& 0x000000000400
#define GST_RING_ACTIVE					& 0x000000000200
#define GST_MODBUS_ACTIVE				& 0x000000000100
#define GST_RESERVED_Y7					& 0x000000000080
#define GST_RESERVED_Y6					& 0x000000000040
#define GST_MACRO_RING_RCVD_BREAK_MSG	& 0x000000000020
#define GST_MACRO_RING_BREAK			& 0x000000000010
#define GST_MACRO_RING_SYN_PACK_FAULT	& 0x000000000008
#define GST_RESERVED_Y2					& 0x000000000004
#define	GST_RESERVED_Y1					& 0x000000000002
#define GST_E_STOP						& 0x000000000001
#define CSNT_AXIS_USED_IN_FEEDRATE		& 0x800000000000
#define CSNT_Z_AXIS_INCREMENT_MODE		& 0x400000000000
#define CSNT_Y_AXIS_USED_IN_FEEDRATE	& 0x200000000000
#define CSNT_Y_AXIS_INCREMENT_MODE		& 0x100000000000
#define CSNT_X_AXIS_USED_IN_FEEDRATE	& 0x080000000000
#define CSNT_X_AXIS_INCREMENT_MODE		& 0x040000000000
#define CSNT_W_AXIS_USED_FEEDRATE		& 0x020000000000
#define CSNT_W_AXIS_INCREMENT_MODE		& 0x010000000000
#define CSNT_V_AXIS_USED_IN_FEEDRATE	& 0x008000000000
#define CSNT_V_AXIS_INCREMENT_MODE		& 0x004000000000
#define CSNT_U_AXIS_USED_FEEDRATE		& 0x002000000000
#define CSNT_U_AXIS_INCREMENT_MODE		& 0x001000000000
#define CSNT_C_AXIS_USED_IN_FEEDRATE	& 0x000800000000
#define CSNT_C_AXIS_INCREMENT_MODE		& 0x000400000000
#define CSNT_B_AXIS_USED_FEEDRATE		& 0x000200000000
#define CSNT_B_AXIS_INCREMENT_MODE		& 0x000100000000
#define CSNT_A_AXIS_USED_IN_FEEDRATE	& 0x000080000000
#define CSNT_A_AXIS_INCREMENT_MODE		& 0x000040000000
#define CSNT_RADIUS_VEC_INCR_MODE		& 0x000020000000
#define CSNT_CONTINUOUS_MOTION_REG		& 0x000010000000
#define CSNT_MOVE_SPEC_BY_TIME			& 0x000008000000
#define CSNT_CONTINUOUS_MOTION			& 0x000004000000
#define CSNT_SINGLE_STEP_MODE			& 0x000002000000
#define CSNT_RUNNING_PROGRAM			& 0x000001000000
#define CSNT_PROGRAM_HOLD_IN_PROGRESS	& 0x000000800000
#define CSNT_RUN_TIME_ERROR				& 0x000000400000
#define CSNT_CIRCLE_RADIUS_ERROR		& 0x000000200000
#define CSNT_AMP_FAULT_ERROR			& 0x000000100000
#define CSNT_FATAL_FOLLOWING_ERROR		& 0x000000080000
#define CSNT_WARNING_FOLLOWING_ERROR	& 0x000000040000
#define CSNT_IN_POSITION				& 0x000000020000
#define CSNT_ROTARY_BUFFER_FULL			& 0x000000010000
#define	CSNT_DELAYED_CALC_FLAG			& 0x000000008000
#define	CSNT_END_OF_BLOCK_STOP_PROGS	& 0x000000004000
#define CSNT_SYNC_M_VAR_ONE_SHOT		& 0x000000002000
#define	CSNT_DWELL_MOVE_BUFFERED		& 0x000000001000
#define CSNT_CTTR_COMP_OUTSIDE_CORNER	& 0x000000000800
#define CSNT_CTTR_COMP_STOP_REQ			& 0x000000000400
#define CSNT_CTTR_COMP_MOVE_BUFFERED	& 0x000000000200
#define CSNT_PRE_JOG_MOVE_IN_PROGS		& 0x000000000100
#define CSNT_SEGMENTED_MOVE				& 0x000000000080
#define CSNT_SEGMENT_ACCEL_FLAG			& 0x000000000040
#define CSNT_SEGMENT_STOP_REQUEST		& 0x000000000020
#define CSNT_PVT_SPLINE_MODE			& 0x000000000010
#define CSNT_CUTTER_COMP_LEFT			& 0x000000000008
#define CSNT_CUTTER_COMP_ON				& 0x000000000004
#define CSNT_CCW_CIRCLE_RAPID_MOVE_MODE	& 0x000000000002
#define CSNT_CIRCLE_SPLINE_MOVE_MODE	& 0x000000000001
#define CST_Z_AXIS_USED_IN_FEEDRATE		& 0x800000000000000000
#define CST_Z_AXIS_INCREMENT_MODE		& 0x400000000000000000
#define CST_Y_AXIS_USED_IN_FEEDRATE		& 0x200000000000000000
#define CST_Y_AXIS_INCREMENT_MODE		& 0x100000000000000000
#define CST_X_AXIS_USED_IN_FEEDRATE		& 0x080000000000000000
#define CST_X_AXIS_INCREMENT_MODE		& 0x040000000000000000
#define CST_W_AXIS_USED_FEEDRATE		& 0x020000000000000000
#define CST_W_AXIS_INCREMENT_MODE		& 0x010000000000000000
#define CST_V_AXIS_USED_IN_FEEDRATE		& 0x008000000000000000
#define CST_V_AXIS_INCREMENT_MODE		& 0x004000000000000000
#define CST_U_AXIS_USED_FEEDRATE		& 0x002000000000000000
#define CST_U_AXIS_INCREMENT_MODE		& 0x001000000000000000
#define CST_C_AXIS_USED_IN_FEEDRATE		& 0x000800000000000000
#define CST_C_AXIS_INCREMENT_MODE		& 0x000400000000000000
#define CST_B_AXIS_USED_FEEDRATE		& 0x000200000000000000
#define CST_B_AXIS_INCREMENT_MODE		& 0x000100000000000000
#define CST_A_AXIS_USED_IN_FEEDRATE		& 0x000080000000000000
#define CST_A_AXIS_INCREMENT_MODE		& 0x000040000000000000
#define CST_RADIUS_VEC_INCR_MODE		& 0x000020000000000000
#define CST_CONTINUOUS_MOTION_REG		& 0x000010000000000000
#define CST_MOVE_SPEC_BY_TIME			& 0x000008000000000000
#define CST_CONTINUOUS_MOTION_MODE		& 0x000004000000000000
#define CST_SINGLE_STEP_MODE			& 0x000002000000000000
#define CST_RUNNING_PROGRAM				& 0x000001000000000000
#define CST_LOOKAHEAD_IN_PROGRES		& 0x000000800000000000
#define CST_RUN_TIME_ERROR				& 0x000000400000000000
#define CST_INTERNAL_MOVE_IN_STACK		& 0x000000200000000000
#define CST_AMP_FAULT_ERROR				& 0x000000100000000000
#define CST_FATAL_FOLLOWING_ERROR		& 0x000000080000000000
#define CST_WARNING_FOLLOWING_ERROR		& 0x000000040000000000
#define CST_IN_POSITION					& 0x000000020000000000
#define CST_ROTARY_BUFFER_FULL			& 0x000000010000000000
#define	CST_DELAYED_CALC_FLAG			& 0x000000008000000000
#define	CST_END_OF_BLOCK_STOP_PROGS		& 0x000000004000000000
#define CST_INTNAL_SYNC_M_VAR_ONE_SHOT	& 0x000000002000000000
#define	CST_INTNAL_DWELL_MOVE_BUFFERED	& 0x000000001000000000
#define CST_CTTR_COMP_OUTSIDE_CORNER	& 0x000000000800000000
#define CST_CTTR_COMP_STOP_REQ			& 0x000000000400000000
#define CST_CTTR_COMP_MOVE_BUFFERED		& 0x000000000200000000
#define CST_PRE_JOG_MOVE_IN_PROGS		& 0x000000000100000000
#define CST_SEGMENTED_MOVE_IN_PROGS		& 0x000000000080000000
#define CST_SEGMENT_ACCEL_FLAG			& 0x000000000040000000
#define CST_SEGMENT_STOP_REQUEST		& 0x000000000020000000
#define CST_PVT_SPLINE_MOVE_MODE		& 0x000000000010000000
#define CST_CUTTER_COMP_LEFT			& 0x000000000008000000
#define CST_CUTTER_COMP_ON				& 0x000000000004000000
#define CST_CCW_CIRCLE_RAPID_MOVE_MODE	& 0x000000000002000000
#define CST_CIRCLE_SPLINE_MOVE_MODE		& 0x000000000001000000
#define CST_LHB_WRAP					& 0x000000000000800000
#define CST_INTERNAL_Y22				& 0x000000000000400000
#define CST_INTERNAL_Y21				& 0x000000000000200000
#define CST_INTERNAL_Y20				& 0x000000000000100000
#define CST_LHB_SYNC_M_VAR_OVERFLOW		& 0x000000000000080000
#define CST_LHB_DIRECTION				& 0x000000000000040000
#define CST_LHB_STOP					& 0x000000000000020000
#define CST_LHB_CHANGE					& 0x000000000000010000
#define CST_LHB_LAST_SEGMENT			& 0x000000000000008000
#define CST_LHB_RECALCULATE				& 0x000000000000004000
#define CST_LHB_FLUSH					& 0x000000000000002000
#define CST_LHB_LAST_MOVE				& 0x000000000000001000
#define CST_LHB_SINGLE_SEGMENT_REGUEST	& 0x000000000000000800
#define CST_LHB_CHANGE_REQUEST			& 0x000000000000000400
#define CST_LHB_MOVE_REQUEST			& 0x000000000000000200
#define CST_LHB_DIRECTION_REQUEST		& 0x000000000000000100
#define CST_RESERVED_Y7					& 0x000000000000000080
#define CST_RESERVED_Y6					& 0x000000000000000040
#define CST_RESERVED_Y5					& 0x000000000000000020
#define CST_RESERVED_Y4					& 0x000000000000000010
#define CST_RADIUS_ERROR				& 0x000000000000000008
#define CST_PROGRAM_RESUME_ERROR		& 0x000000000000000004
#define CST_DESIRED POSITION_LIMIT_STOP	& 0x000000000000000002
#define CST_IN_PROGRAM_PMATCH			& 0x000000000000000001
#define MAXLINKS            68               // max number of PLCC links
#define MINLINKS            41
#define MAXLDS              27               // max number of PLCC LADDER DIAG links
#define MINLDS              27               // MIN number of PLCC LADDER DIAG links
#define MAXSTRLINK          ((8*MAXLINKS)+1) // max string length of links
#define VERSION_BUFFER_SIZE 100
#define DATE_BUFFER_SIZE    100
#define ISR_IPOS            1                /*         IR0 for in position          */
#define ISR_BREQ            2                /*         IR1 for buffer request       */
#define ISR_FERROR          4                /*         IR2 for general error        */
#define ISR_ERROR           8                /*         IR3 for following error      */
#define ISR_HREQ            16               /*         IR4 for communication        */
#define ISR_IR5             32               /*         IR5                          */
#define ISR_IR6             64               /*         IR6                          */
#define ISR_IR7             128              /*         IR7                          */
#define COMM_CHARS(c)      (c & 0x0FFFFFFF)
#define COMM_STATUS(c)     (c & 0xF0000000)
#define COMM_OK			   1
#define COMM_EOT           0x80000000
#define COMM_TIMEOUT       0xC0000000
#define COMM_BADCKSUM      0xD0000000
#define COMM_ERROR         0xE0000000
#define COMM_FAIL          0xF0000000
#define COMM_ANYERR        0x70000000
#define COMM_UNSOLICITED   0x10000000
#define IS_COMM_MORE(c)     ((c & COMM_FAIL) == 0)
#define IS_COMM_EOT(c)      ((c & COMM_FAIL) == COMM_EOT)
#define IS_COMM_TIMEOUT(c)  ((c & COMM_FAIL) == COMM_TIMEOUT)
#define IS_COMM_BADCKSUM(c) ((c & COMM_FAIL) == COMM_BADCKSUM)
#define IS_COMM_ERROR(c)    ((c & COMM_FAIL) == COMM_ERROR)
#define IS_COMM_FAIL(c)     ((c & COMM_FAIL) == COMM_FAIL)
#define IS_COMM_ANYERROR(c) ((c & COMM_ANYERR) > 0)
#define IS_COMM_UNSOLICITED(c) ((c & 0xF0000000) == COMM_UNSOLICITED)
#define ON              TRUE
#define OFF             FALSE
#define RETRY           5
#define MAXAOUT         160     // ASCII interface max output string length
#define MAXAIN          256  // ASCII interface max input string length
#define MAXCH           256
#define	MAX_LINE	( 256 )

#define	MAX_M_CODE	( 1000 )

#define	MAX_G_GROUP	( 27 )

#define	MAX_ADDRESS	( 27 )

#define	MAX_PLANES	( 3 )

#define	MAX_AXIS	( 9 )

#define	MAX_COORD_SYS	( 2 )

#define	MAX_POSITION_TYPES	( 6 )

#define	MAX_LOCAL_VARS	( 100 )

#define	MAX_COMMON_VARS	( 400 )

#define	MAX_STATIC_VARS	( 500 )

#define	MAX_DISPERRORS	( 100 )

#define	MAX_TOOL	( 300 )

#define	NUM_TOOL_TYPES	( 8 )

#define	NUM_TOOL_MATRL	( 3 )

#define	NUMCOORDTYPES	( 26 )

#define	NUMEXTWORKCOORDS	( 48 )

#define	MAXSTRINGLEN	( 84 )

#define	MAXRS274STRING	( 128 )

#define	MAX_CODEERRORS	( 5 )

#define	MAX_COMMERRORS	( 5 )

#define	MAXAXISSTR	( 12 )

#define	MAX_NESTING	( 10 )

#define	DPRNT_LINES	( 40 )

#define	DPRNT_LINE_MAX	( 80 )

#define	ERR_VBG_MAXUSERS	( -100 )

#define	ERR_VBG_MAXENTRIES	( -101 )

#define	ERR_VBG_NOENTRIES	( -102 )

#define GATHER_COM1    0x00000001L
#define GATHER_COM2    0x00000002L
#define GATHER_COM3    0x00000004L
#define GATHER_COM4    0x00000008L
#define GATHER_COM5    0x00000010L
#define GATHER_COM6    0x00000020L
#define GATHER_COM7    0x00000040L
#define GATHER_COM8    0x00000080L
#define GATHER_ENC1    0x00000100L
#define GATHER_ENC2    0x00000200L
#define GATHER_ENC3    0x00000400L
#define GATHER_ENC4    0x00000800L
#define GATHER_ENC5    0x00001000L
#define GATHER_ENC6    0x00002000L
#define GATHER_ENC7    0x00004000L
#define GATHER_ENC8    0x00008000L
#define GATHER_DAC1    0x00010000L
#define GATHER_DAC2    0x00020000L
#define GATHER_DAC3    0x00040000L
#define GATHER_DAC4    0x00080000L
#define GATHER_DAC5    0x00100000L
#define GATHER_DAC6    0x00200000L
#define GATHER_DAC7    0x00400000L
#define GATHER_DAC8    0x00800000L
#define GATHER_CUR1    0x01000000L
#define GATHER_CUR2    0x02000000L
#define GATHER_CUR3    0x04000000L
#define GATHER_CUR4    0x08000000L
#define GATHER_CUR5    0x10000000L
#define GATHER_CUR6    0x20000000L
#define GATHER_CUR7    0x40000000L
#define GATHER_CUR8    0x80000000L
#define MAXADRLEN       15
#define DEF_PCBUSTYPE          0 // ISA, VME etc.
#define DEF_TYPE               1 // PMAC 1
#define DEF_PORT             528 // 0x210
#define DEF_INTERRUPT          0 // No interrupt, make them select!
#define DEF_FIFO             0x0
#define DEF_LOCATION           0 // Computer BUS
#define DEF_TIMEOUT          200
#define DEF_FLUSHTIMEOUT      20
#define DEF_DPRROT0ADR     0x400
#define DEF_DPRROT1ADR     0x400
#define DEF_DPRROT2ADR       0x0
#define DEF_DPRROT3ADR       0x0
#define DEF_DPRROT4ADR       0x0
#define DEF_DPRROT5ADR       0x0
#define DEF_DPRROT6ADR       0x0
#define DEF_DPRROT7ADR       0x0
#define DEF_DPRROT8ADR       0x0
#define DEF_DPRROT9ADR       0x0
#define DEF_DPRROT10ADR      0x0
#define DEF_DPRROT11ADR      0x0
#define DEF_DPRROT12ADR      0x0
#define DEF_DPRROT13ADR      0x0
#define DEF_DPRROT14ADR      0x0
#define DEF_DPRROT15ADR      0x0
#define DEF_DPRGATSIZE       0x0
#define DEF_DPRVARRDSIZE     0x180
#define DEF_DPRVARWRSIZE     0x20
#define DEF_DPRVARADR      0xD60
#define DEF_DPRUSERBUF     0x580
#define DEF_FBSTART       0x9f00
#define DEF_FBTIMER       0x9f80
#define DEF_BAUDRATE       38400
#define DEF_COMPORT            0 // COM1
#define DEF_PARITY             0 // No Parity
#define DEF_RTS_CONTROL        1 // Enabled
#define DEF_CTS_OUTPUT         1 // Checked
#define DEF_VMEBASE     0x7FA000 // PMAC's Default VME base address
#define DEF_VMEDPRBASE  0x700000 // PMAC's Default VME DPR Base Address
#define DEF_VMEINTERRUPT       7 // PMAC's Default VME Interrupt Level
#define DEF_VMEHOSTID          0 // VMIC 7686
#define DEF_VMEAM           0x39 // Address Modifier
#define DEF_VMEAMDC         0x04 // Address Modifier Dont care
#define DEF_VMEIRQVECT      0xA1 // VME IRQ Vector
#define DEF_BUSCHARTIMEOUT    1000
#define DEF_BUSFLUSHTIMEOUT   10
#define DEF_VMECHARTIMEOUT    30
#define DEF_VMEFLUSHTIMEOUT    3
#define DEF_DPBKTIMEOUT       10
#define DEF_DPRTTIMEOUT        2
#define DEF_DPCHARTIMEOUT   1000
#define DEF_DPFLUSHTIMEOUT    20
#define DEF_SERCHARTIMEOUT  2000
#define DEF_SERFLUSHTIMEOUT   30
#define DEF_SERWRITECONSTANTTIMEOUT 1000
#define DEF_SERWRITEMULTIPLYERTIMEOUT   0
#define DEF_SERREADCONSTANTTIMEOUT   30
#define DEF_SERREADINTERVALTIMEOUT   30 // A VALUE OF 0 indicates interval timeouts are not used
#define DEF_SERREADCONSTANTTIMEOUTSLOW   30
#define DEF_SERREADINTERVALTIMEOUTSLOW   30 // A VALUE OF 0 indicates interval timeouts are not used
#define DEF_SERREADMULTIPLYERTIMEOUT  0
#define DEF_USBMODE                   0
#define DEF_IPADDRESS          0xc0065e05
#define DEF_DISABLEIRQ         1
#define DEF_DISABLEDPRAM       1
#define DEF_USENEWCOMMFEATURES 1 // Use New Firmware features by default
#define SYS_FATAL_FERR    0X0001
#define SYS_WARN_FERR     0X0002
#define SYS_SERVO_ERR     0X0004
#define SYS_REENTRY_ERR   0X0008
#define SYS_MEM_CHKSUM    0X0010
#define SYS_PROM_CHKSUM   0X0020
#define SYS_RUNTIME_ERR   0X0040
#define SYS_CIR_RAD_ERR   0X0080
#define SYS_AMP_FAULT     0X0100
#define SYS_RING_ERR      0X0200
#define SYS_RING_IO_ERR   0X0400
#define ERR_BAUDSEARCH    -27
#define ERR_VBG_DPRNOTON  -28
#define CHECKSUMACKOK     -20 // good acknowlegde checksum
#define CHECKSUMDATAOK    -21 // good data checksum
#define CHECKSUMTIMEOUT   -22 // it times out
#define CHECKSUMDATABAD   -23 // bad data checksum
#define CHECKSUMACKBAD    -24 // bad command acknowledge checksum
#define	PARAM_TYPE	( "PmacType" )

#define	PARAM_PCBUSTYPE	( "PCBusType" )

#define	PARAM_PORT	( "Port" )

#define	PARAM_INTERRUPT	( "Interrupt" )

#define	PARAM_BUSCHARTIMEOUT	( "BUSCharTimeout" )

#define	PARAM_BUSFLUSHTIMEOUT	( "BUSFlushTimeout" )

#define	PARAM_FIFO	( "DualPortRam" )

#define	PARAM_ERROR	( "InstallError" )

#define	PARAM_DPRROT0	( "DualPortRamRot0" )

#define	PARAM_DPRROT1	( "DualPortRamRot1" )

#define	PARAM_DPRROT2	( "DualPortRamRot2" )

#define	PARAM_DPRROT3	( "DualPortRamRot3" )

#define	PARAM_DPRROT4	( "DualPortRamRot4" )

#define	PARAM_DPRROT5	( "DualPortRamRot5" )

#define	PARAM_DPRROT6	( "DualPortRamRot6" )

#define	PARAM_DPRROT7	( "DualPortRamRot7" )

#define	PARAM_DPRROT8	( "DualPortRamRot8" )

#define	PARAM_DPRROT9	( "DualPortRamRot9" )

#define	PARAM_DPRROT10	( "DualPortRamRot10" )

#define	PARAM_DPRROT11	( "DualPortRamRot11" )

#define	PARAM_DPRROT12	( "DualPortRamRot12" )

#define	PARAM_DPRROT13	( "DualPortRamRot13" )

#define	PARAM_DPRROT14	( "DualPortRamRot14" )

#define	PARAM_DPRROT15	( "DualPortRamRot15" )

#define	PARAM_DPRGATSIZE	( "DualPortRamGatherSize" )

#define	PARAM_DPRVARRDSIZE	( "DualPortRamVarReadSize" )

#define	PARAM_DPRVARWRSIZE	( "DualPortRamVarWriteSize" )

#define	PARAM_DPRVARADR	( "DualPortRamVar" )

#define	PARAM_DPRUSERSIZE	( "DualPortRamUserSize" )

#define	PARAM_FBSTART	( "FunctionBlockStart" )

#define	PARAM_FBTIMER	( "FunctionBlockTimer" )

#define	PARAM_PORTNUMBER	( "SerialPortNumber" )

#define	PARAM_BAUDRATE	( "SerialBaudrate" )

#define	PARAM_PARITY	( "SerialParity" )

#define	PARAM_CTS	( "SerialCTSOutput" )

#define	PARAM_RTS	( "SerialRTSControl" )

#define	PARAM_LOCATION	( "Location" )

#define	PARAM_ENUMERATION	( "Enumeration" )

#define	PARAM_DISABLEDPR	( "DisableDPRAM" )

#define	PARAM_DISABLEIRQ	( "DisableIRQ" )

#define	PARAM_PCI_SLOT	( "PCISlot" )

#define	PARAM_PCI_BUS	( "PCIBus" )

#define	PARAM_IPADDRESS	( "IPAddress" )

#define	PARAM_USENEWCOMMFEATURES	( "UseNewCommFeatures" )

#define	PARAM_USBMODE	( "USBMode" )

#define	PARAM_DPRAUTOACTIVE	( "DPRAutoActive" )

#define	PARAM_DPRRTACTIVE	( "DPRRealTimeActive" )

#define	PARAM_DPRBGACTIVE	( "DPRBackgroundActive" )

#define	PARAM_DPRBGPERIOD	( "DPRBackgroundPeriod" )

#define	PARAM_DPRRTPERIOD	( "DPRRealTimePeriod" )

#define	PARAM_DPRRTMTRMASK	( "DPRRealTimeMtrMask" )

#define	PARAM_DPRBGCOORDSYS	( "DPRBackgroundCrdSys" )

#define	PARAM_DPRNUMMOTORS	( "DPRNumMotors" )

#define	PARAM_DPRMONITORPERIOD	( "DPRMonitorPeriod" )

#define	PARAM_ENABLEINTR	( "EnableInterrupts" )

#define	PARAM_DPBKTIMEOUT	( "DPRBackTimeout" )

#define	PARAM_DPRTTIMEOUT	( "DPRRTTimeout" )

#define	PARAM_DPCHARTIMEOUT	( "DPRCharTimeout" )

#define	PARAM_DPFLUSHTIMEOUT	( "DPRFlushTimeout" )

#define	PARAM_VMEBASE	( "VMEBase" )

#define	PARAM_VMEDPRBASE	( "VMEDPRBase" )

#define	PARAM_VMEINTERRUPT	( "VMEInterrupt" )

#define	PARAM_VMEHOSTID	( "VMEHostID" )

#define	PARAM_VMEAM	( "VMEAM" )

#define	PARAM_VMEAMDC	( "VMEAMDontCare" )

#define	PARAM_VMEIRQVECT	( "VMEIRQVector" )

#define	PARAM_VMECHARTIMEOUT	( "VMECharTimeout" )

#define	PARAM_VMEFLUSHTIMEOUT	( "VMEFlushTimeout" )

#define	PARAM_SERWRITEMULTIPLYERTIMEOUT	( "SerWriteMultiplyerTimeout" )

#define	PARAM_SERWRITECONSTANTTIMEOUT	( "SerWriteConstantTimeout" )

#define	PARAM_SERREADMULTIPLYERTIMEOUT	( "SerReadMultiplyerTimeout" )

#define	PARAM_SERREADCONSTANTTIMEOUT	( "SerReadConstantTimeout" )

#define	PARAM_SERREADINTERVALTIMEOUT	( "SerReadIntervalTimeout" )

#define	PARAM_SERCHARTIMEOUT	( "SerCharTimeout" )

#define	PARAM_SERFLUSHTIMEOUT	( "SerFlushTimeout" )

#define	PARAM_NCTITLE	( "Title" )

#define	PARAM_TOOLPROFILE	( "ToolProfile" )

#define	PARAM_COORDPROFILE	( "CoordProfile" )

#define	PARAM_MACHINETYPE	( "MachineType" )

#define	PARAM_NOOFTOOLS	( "NoOfTools" )

#define	PARAM_NOOFBLOCKS	( "NoOfBlocks" )

#define	PARAM_NOOFCOORDSYS	( "NoOfCoordSys" )

#define	PARAM_LEASTHANDLEINC	( "LeastHandleInc" )

#define	PARAM_MAXHANDLEINC	( "MaxHandleInc" )

#define	PARAM_LEASTJOGINC	( "LeastJogInc" )

#define	PARAM_AXISMOTORMAP	( "AxisMotorMap" )

#define	PARAM_AXISMOTORSEL	( "AxisMotorSel" )

#define	PARAM_AXISDISPMAP	( "AxisDispMap" )

#define	PARAM_MAXRAPIDOVRD	( "MaxRapidOvrd" )

#define	PARAM_MAXFEEDOVRD	( "MaxFeedOvrd" )

#define	PARAM_ISSPINDLE	( "IsSpindle" )

#define	PARAM_HASSLAVE	( "HasSlave" )

#define	PARAM_ISPHANTOM	( "IsPhantom" )

#define	PARAM_DISPLAY	( "Display" )

#define	PARAM_DISPLAYSLAVE	( "DisplaySlave" )

#define	PARAM_HOMEMODE	( "HomeMode" )

#define	PARAM_HOMEPRGNUMBER	( "HomePrgNumber" )

#define	PARAM_PRECISION	( "Precision" )

#define	PARAM_METRICDISPLAY	( "MetricDisplay" )

#define	PARAM_METRICUNITS	( "MetricUnits" )

#define	PARAM_PROBEPRGNUMBER	( "ProbePrgNumber" )

#define	PARAM_PULSEPERUNIT	( "PulsePerInit" )

#define	PARAM_INPOSITIONBAND	( "InpositionBand" )

#define	PARAM_MAXRAPID	( "MaxRapid" )

#define	PARAM_MAXFEED	( "MaxFeed" )

#define	PARAM_FATALFERROR	( "FatalFError" )

#define	PARAM_WARNFERROR	( "WarnFError" )

#define	PARAM_JOGSPEEDLOW	( "JogSpeedLow" )

#define	PARAM_JOGSPEEDMedLow	( "JogSpeedMedLow" )

#define	PARAM_JOGSPEEDMed	( "JogSpeedMed" )

#define	PARAM_JOGSPEEDMedHigh	( "JogSpeedMedHigh" )

#define	PARAM_JOGSPEEDHigh	( "JogSpeedHigh" )

#define	PARAM_FORMATINCH	( "FormatInch" )

#define	PARAM_FORMATMM	( "FormatMM" )

typedef struct _SERVOSTATUSTURBO
    {
    USHORT rapid_spd_sel	: 1;
    USHORT dac_sign_mag	: 1;
    USHORT sw_capture	: 1;
    USHORT fe_capture	: 1;
    USHORT handwheel_ena	: 1;
    USHORT hw_mode	: 1;
    USHORT phased_motor	: 1;
    USHORT yenc_phase	: 1;
    USHORT user_servo	: 1;
    USHORT user_phase	: 1;
    USHORT home_search	: 1;
    USHORT block_request	: 1;
    USHORT limit_stop	: 1;
    USHORT desired_vel_0	: 1;
    USHORT data_block_err	: 1;
    USHORT dwelling	: 1;
    USHORT integrator_ena	: 1;
    USHORT run_program	: 1;
    USHORT open_loop	: 1;
    USHORT amp_enabled	: 1;
    USHORT algo_ena	: 1;
    USHORT pos_limit	: 1;
    USHORT neg_limit	: 1;
    USHORT activated	: 1;
    USHORT pads	: 8;
    } 	SERVOSTATUSTURBO;

typedef struct _MOTORSTATUSTURBO
    {
    USHORT in_position	: 1;
    USHORT warn_ferr	: 1;
    USHORT fatal_ferr	: 1;
    USHORT amp_fault	: 1;
    USHORT backlash_dir	: 1;
    USHORT amp_i2t_err	: 1;
    USHORT integral_ferr	: 1;
    USHORT triger_home_flg	: 1;
    USHORT phase_find_err	: 1;
    USHORT tbd09	: 1;
    USHORT home_complete	: 1;
    USHORT stopped_on_limit	: 1;
    USHORT 	: 1;
    USHORT 	: 1;
    USHORT 	: 1;
    USHORT cs_assigned	: 1;
    USHORT cs_def	: 4;
    USHORT coord_sys	: 4;
    USHORT padm	: 8;
    } 	MOTORSTATUSTURBO;

typedef struct _REALMTURBO
    {
    long pepos[ 2 ];
    long pdac;
    SERVOSTATUSTURBO sstatus;
    MOTORSTATUSTURBO mstatus;
    long bpos[ 2 ];
    long fvel;
    long hwpos[ 2 ];
    long pos[ 2 ];
    } 	REALMTURBO;

typedef struct _FGBUFFERTURBO
    {
    USHORT 	: 16;
    USHORT 	: 8;
    USHORT 	: 7;
    USHORT hostbusy	: 1;
    USHORT servotimer;
    USHORT servotimerms8	: 8;
    USHORT pspare	: 7;
    USHORT dataready	: 1;
    long motor_mask;
    REALMTURBO motor[ 32 ];
    } 	FGBUFFERTURBO;

typedef struct _SERVOSTATUS
    {
    USHORT internal1	: 8;
    USHORT internal2	: 2;
    USHORT home_search	: 1;
    USHORT block_request	: 1;
    USHORT rffu1	: 1;
    USHORT desired_vel_0	: 1;
    USHORT data_block_err	: 1;
    USHORT dwelling	: 1;
    USHORT integration	: 1;
    USHORT run_program	: 1;
    USHORT open_loop	: 1;
    USHORT phased_motor	: 1;
    USHORT handwheel_ena	: 1;
    USHORT pos_limit	: 1;
    USHORT neg_limit	: 1;
    USHORT activated	: 1;
    USHORT pad	: 8;
    } 	SERVOSTATUS;

typedef struct _GLOBALSTATUS
    {
    USHORT rffu2	: 8;
    USHORT internal1	: 3;
    USHORT buffer_full	: 1;
    USHORT internal2	: 3;
    USHORT dpram_response	: 1;
    USHORT plc_command	: 1;
    USHORT plc_buf_open	: 1;
    USHORT rot_buf_open	: 1;
    USHORT prog_buf_open	: 1;
    USHORT internal3	: 2;
    USHORT host_comm_mode	: 1;
    USHORT internal4	: 1;
    USHORT pad2	: 8;
    USHORT card_adrssed	: 1;
    USHORT all_adrssed	: 1;
    USHORT rffu1	: 2;
    USHORT ring_error	: 1;
    USHORT ring_io_error	: 1;
    USHORT tws_error	: 1;
    USHORT end_gather	: 1;
    USHORT rapid_m_flag	: 1;
    USHORT rti_warning	: 1;
    USHORT earom_error	: 1;
    USHORT dpram_error	: 1;
    USHORT prom_checksum	: 1;
    USHORT mem_checksum	: 1;
    USHORT comp_on	: 1;
    USHORT stimulate_on	: 1;
    USHORT stimulus_ent	: 1;
    USHORT prep_trig_gat	: 1;
    USHORT prep_next_serv	: 1;
    USHORT data_gat_on	: 1;
    USHORT servo_err	: 1;
    USHORT servo_active	: 1;
    USHORT intr_reentry	: 1;
    USHORT intr_active	: 1;
    USHORT pad1	: 8;
    } 	GLOBALSTATUS;

typedef struct _REALM
    {
    long dpos[ 2 ];
    long apos[ 2 ];
    long hwpos[ 2 ];
    long cpos[ 2 ];
    unsigned long pdac;
    SERVOSTATUS sstatus;
    long fvel;
    long movtim;
    unsigned long mspare[ 3 ];
    } 	REALM;

typedef struct _FGBUFFER
    {
    USHORT hostbusy	: 1;
    USHORT rffu1	: 15;
    USHORT servotimer	: 15;
    USHORT pmacbusy	: 1;
    GLOBALSTATUS globalstatus;
    long ffdrate[ 2 ][ 2 ];
    unsigned long spareglobal[ 2 ];
    REALM mtrcrd[ 8 ];
    } 	FGBUFFER;

typedef struct _GLOBALSTATUSTURBO
    {
    USHORT rffu2	: 8;
    USHORT internal1	: 3;
    USHORT buffer_full	: 1;
    USHORT internal2	: 4;
    USHORT internal3	: 1;
    USHORT plc_buf_open	: 1;
    USHORT rot_buf_open	: 1;
    USHORT prog_buf_open	: 1;
    USHORT bin_rot_buf_open	: 1;
    USHORT rffu3	: 1;
    USHORT vme	: 1;
    USHORT ultralite	: 1;
    USHORT pad2	: 8;
    USHORT card_adrssed	: 1;
    USHORT all_adrssed	: 1;
    USHORT rffu1	: 2;
    USHORT ring_error	: 1;
    USHORT ring_io_error	: 1;
    USHORT tws_error	: 1;
    USHORT end_gather	: 1;
    USHORT rapid_m_flag	: 1;
    USHORT rti_warning	: 1;
    USHORT earom_error	: 1;
    USHORT dpram_error	: 1;
    USHORT prom_checksum	: 1;
    USHORT mem_checksum	: 1;
    USHORT comp_on	: 1;
    USHORT wdt1	: 1;
    USHORT wdt2	: 1;
    USHORT ext_trig_gat	: 1;
    USHORT prep_trig_gat	: 1;
    USHORT data_gat_on	: 1;
    USHORT servo_err	: 1;
    USHORT servo_active	: 1;
    USHORT intr_reentry	: 1;
    USHORT intr_active	: 1;
    USHORT pad1	: 8;
    } 	GLOBALSTATUSTURBO;

typedef struct _MOTORSTATUS
    {
    USHORT in_position	: 1;
    USHORT warn_ferr	: 1;
    USHORT fatal_ferr	: 1;
    USHORT amp_fault	: 1;
    USHORT backlash_dir	: 1;
    USHORT amp_i2t_err	: 1;
    USHORT integral_ferr	: 1;
    USHORT triger_home_flg	: 1;
    USHORT phase_find_err	: 1;
    USHORT rffu2	: 1;
    USHORT home_complete	: 1;
    USHORT stopped_on_limit	: 1;
    USHORT rffu3	: 2;
    USHORT amp_enabled	: 1;
    USHORT rffu4	: 1;
    USHORT rffu5	: 4;
    USHORT coord_sys	: 3;
    USHORT cs_assigned	: 1;
    USHORT pad	: 8;
    } 	MOTORSTATUS;

typedef struct _PROGRAMSTATUS
    {
    USHORT cir_spline_move	: 1;
    USHORT ccw_move	: 1;
    USHORT cc_on	: 1;
    USHORT cc_left	: 1;
    USHORT pvt_spline_move	: 1;
    USHORT seg_stop_request	: 1;
    USHORT seg_accel	: 1;
    USHORT seg_move	: 1;
    USHORT rapid_move_mode	: 1;
    USHORT cc_buffered	: 1;
    USHORT cc_stop_request	: 1;
    USHORT cc_outside_corner	: 1;
    USHORT dwell_buffered	: 1;
    USHORT sync_m_func	: 1;
    USHORT eob_stop	: 1;
    USHORT delayed_calc	: 1;
    USHORT rot_buff_full	: 1;
    USHORT in_position	: 1;
    USHORT warn_ferr	: 1;
    USHORT fatal_ferr	: 1;
    USHORT amp_fault	: 1;
    USHORT circle_rad_err	: 1;
    USHORT run_time_err	: 1;
    USHORT prog_hold	: 1;
    USHORT pad	: 8;
    } 	PROGRAMSTATUS;

typedef struct _COORDSTATUS
    {
    unsigned long motor_def;
    USHORT prog_running	: 1;
    USHORT single_step_mode	: 1;
    USHORT cont_motion_mode	: 1;
    USHORT tm_mode	: 1;
    USHORT cont_motion_req	: 1;
    USHORT rad_vect_inc_mode	: 1;
    USHORT a_axis_inc	: 1;
    USHORT a_axis_infeed	: 1;
    USHORT b_axis_inc	: 1;
    USHORT b_axis_infeed	: 1;
    USHORT c_axis_inc	: 1;
    USHORT c_axis_infeed	: 1;
    USHORT u_axis_inc	: 1;
    USHORT u_axis_infeed	: 1;
    USHORT v_axis_inc	: 1;
    USHORT v_axis_infeed	: 1;
    USHORT w_axis_inc	: 1;
    USHORT w_axis_infeed	: 1;
    USHORT x_axis_inc	: 1;
    USHORT x_axis_infeed	: 1;
    USHORT y_axis_inc	: 1;
    USHORT y_axis_infeed	: 1;
    USHORT z_axis_inc	: 1;
    USHORT z_axis_infeed	: 1;
    USHORT pad2	: 8;
    } 	COORDSTATUS;

typedef struct _BGMOTOR
    {
    long mpos[ 2 ];
    long bpos[ 2 ];
    MOTORSTATUS mstatus;
    COORDSTATUS cstatus;
    long cpos[ 9 ][ 2 ];
    PROGRAMSTATUS pstatus;
    unsigned long pr;
    unsigned long timrem;
    long tats;
    unsigned long pe;
    long fvel;
    } 	BGMOTOR;

typedef struct _BKFDPTR
    {
    USHORT fdadr	: 16;
    USHORT pad3	: 6;
    USHORT fdhld	: 1;
    USHORT pad2	: 1;
    USHORT pad1	: 8;
    } 	BKFDPTR;

typedef struct _BGBUFFER
    {
    USHORT dataready;
    USHORT servo;
    unsigned long cpanelport;
    unsigned long thumbwport;
    unsigned long machineport;
    BKFDPTR fdptr[ 2 ];
    long fdpot[ 2 ];
    unsigned long spare2[ 1 ];
    BGMOTOR mtrcrd[ 8 ];
    } 	BGBUFFER;

typedef struct _BGBUFFER *PBGBUFFER;

typedef struct _BKFDPTRTURBO
    {
    unsigned long fdadr	: 19;
    USHORT pad3	: 2;
    USHORT fdslew	: 1;
    USHORT fdhld	: 1;
    USHORT pad2	: 1;
    USHORT pad1	: 8;
    } 	BKFDPTRTURBO;

typedef struct _COORDSTATUSTURBO
    {
    USHORT in_prog_pmatch	: 1;
    USHORT desired_position_limit	: 1;
    USHORT program_resume_error	: 1;
    USHORT radius_error	: 1;
    USHORT reserved	: 4;
    USHORT lhb_direction_request	: 1;
    USHORT lhb_move_request	: 1;
    USHORT lhb_change_request	: 1;
    USHORT lhb_sing_seg_request	: 1;
    USHORT lhb_lasr_move	: 1;
    USHORT lhb_flush	: 1;
    USHORT lhb_recalculate	: 1;
    USHORT lhb_last_segment	: 1;
    USHORT lhb_change	: 1;
    USHORT lhb_stop	: 1;
    USHORT lhb_direction	: 1;
    USHORT lhb_sync_m_var_ovrflow	: 1;
    USHORT internal	: 3;
    USHORT look_ahead_buf_wrap	: 1;
    USHORT pading	: 8;
    USHORT prog_running	: 1;
    USHORT single_step_mode	: 1;
    USHORT cont_motion_mode	: 1;
    USHORT tm_mode	: 1;
    USHORT cont_motion_req	: 1;
    USHORT rad_vect_inc_mode	: 1;
    USHORT a_axis_inc	: 1;
    USHORT a_axis_infeed	: 1;
    USHORT b_axis_inc	: 1;
    USHORT b_axis_infeed	: 1;
    USHORT c_axis_inc	: 1;
    USHORT c_axis_infeed	: 1;
    USHORT u_axis_inc	: 1;
    USHORT u_axis_infeed	: 1;
    USHORT v_axis_inc	: 1;
    USHORT v_axis_infeed	: 1;
    USHORT w_axis_inc	: 1;
    USHORT w_axis_infeed	: 1;
    USHORT x_axis_inc	: 1;
    USHORT x_axis_infeed	: 1;
    USHORT y_axis_inc	: 1;
    USHORT y_axis_infeed	: 1;
    USHORT z_axis_inc	: 1;
    USHORT z_axis_infeed	: 1;
    USHORT pad2	: 8;
    } 	COORDSTATUSTURBO;

typedef struct _BACKCTURBO
    {
    long ffdrate[ 2 ];
    long fdpot;
    BKFDPTR fdptr;
    COORDSTATUSTURBO cstatus;
    long cpos[ 9 ][ 2 ];
    PROGRAMSTATUS pstatus;
    unsigned long pr;
    unsigned long timrem;
    long tats;
    unsigned long pe;
    unsigned long sparec[ 3 ];
    } 	BACKCTURBO;

typedef struct _BGBUFFERTURBO
    {
    USHORT cordsys	: 8;
    USHORT spare1	: 8;
    USHORT spare2	: 8;
    USHORT spare3	: 7;
    USHORT hostbusy	: 1;
    USHORT servotimer;
    USHORT servotimerms8	: 8;
    USHORT pspare	: 7;
    USHORT datardy	: 1;
    unsigned long cpanelport;
    unsigned long machineport;
    unsigned long thumbwport;
    GLOBALSTATUSTURBO globalstatus;
    unsigned long sparebg[ 3 ];
    BACKCTURBO crdsys[ 16 ];
    } 	BGBUFFERTURBO;

#define CNC_FIRST               5000
#define CNC_SETOPEREXT          (CNC_FIRST + 0)
#define CNC_SETOPERDEMO         (CNC_FIRST + 1)
#define CNC_SPARE1              (CNC_FIRST + 2)
#define CNC_MODEINDEX           (CNC_FIRST + 3)
#define CNC_SETMODEAUTO         (CNC_FIRST + 4)
#define CNC_SETMODEMANUAL       (CNC_FIRST + 5)
#define CNC_SETMODEMDI          (CNC_FIRST + 6)
#define CNC_JOGPLUS             (CNC_FIRST + 7)
#define CNC_JOGMINUS            (CNC_FIRST + 8)
#define CNC_JOGRETURN           (CNC_FIRST + 9)
#define CNC_JOGSTOP             (CNC_FIRST + 10)
#define CNC_TURRETCW            (CNC_FIRST + 11)
#define CNC_TURRETCCW           (CNC_FIRST + 12)
#define CNC_CHUCKRELEASE        (CNC_FIRST + 13)
#define CNC_CHUCKCLOSE          (CNC_FIRST + 14)
#define CNC_CHUCKTOGGLE         (CNC_FIRST + 15)
#define CNC_COOLANTINDEX        (CNC_FIRST + 16)
#define CNC_COOLANTMISTON       (CNC_FIRST + 17)
#define CNC_COOLANTMISTOFF      (CNC_FIRST + 18)
#define CNC_COOLANTMISTTOGGLE   (CNC_FIRST + 19)
#define CNC_COOLANTFLOODON      (CNC_FIRST + 20)
#define CNC_COOLANTFLOODOFF     (CNC_FIRST + 21)
#define CNC_COOLANTFLOODTOGGLE  (CNC_FIRST + 22)
#define CNC_SINGLEBLOCKON       (CNC_FIRST + 23)
#define CNC_SINGLEBLOCKOFF      (CNC_FIRST + 24)
#define CNC_SINGLEBLOCKTOGGLE   (CNC_FIRST + 25)
#define CNC_BLOCKDELETEON       (CNC_FIRST + 26)
#define CNC_BLOCKDELETEOFF      (CNC_FIRST + 27)
#define CNC_BLOCKDELETETOGGLE   (CNC_FIRST + 28)
#define CNC_OPTIONALSTOPON      (CNC_FIRST + 29)
#define CNC_OPTIONSTOPOFF       (CNC_FIRST + 30)
#define CNC_OPTIONSTOPTOGGLE    (CNC_FIRST + 31)
#define CNC_USER1ON             (CNC_FIRST + 32)
#define CNC_USER1OFF            (CNC_FIRST + 33)
#define CNC_USER1TOGGLE         (CNC_FIRST + 34)
#define CNC_USER2ON             (CNC_FIRST + 35)
#define CNC_USER2OFF            (CNC_FIRST + 36)
#define CNC_USER2TOGGLE         (CNC_FIRST + 37)
#define CNC_USER3ON             (CNC_FIRST + 38)
#define CNC_USER3OFF            (CNC_FIRST + 39)
#define CNC_USER3TOGGLE         (CNC_FIRST + 40)
#define CNC_USER4ON             (CNC_FIRST + 41)
#define CNC_USER4OFF            (CNC_FIRST + 42)
#define CNC_USER4TOGGLE         (CNC_FIRST + 43)
#define CNC_USER5ON             (CNC_FIRST + 44)
#define CNC_USER5OFF            (CNC_FIRST + 45)
#define CNC_USER5TOGGLE         (CNC_FIRST + 46)
#define CNC_USER6ON             (CNC_FIRST + 47)
#define CNC_USER6OFF            (CNC_FIRST + 48)
#define CNC_USER6TOGGLE         (CNC_FIRST + 49)
#define CNC_USER7ON             (CNC_FIRST + 50)
#define CNC_USER7OFF            (CNC_FIRST + 51)
#define CNC_USER7TOGGLE         (CNC_FIRST + 52)
#define CNC_USER8ON             (CNC_FIRST + 53)
#define CNC_USER8OFF            (CNC_FIRST + 54)
#define CNC_USER8TOGGLE         (CNC_FIRST + 55)
#define CNC_AXISINDEX           (CNC_FIRST + 56)
#define CNC_SELAXISX            (CNC_FIRST + 57)
#define CNC_SELAXISY            (CNC_FIRST + 58)
#define CNC_SELAXISZ            (CNC_FIRST + 59)
#define CNC_SELAXISA            (CNC_FIRST + 60)
#define CNC_SELAXISB            (CNC_FIRST + 61)
#define CNC_SELAXISC            (CNC_FIRST + 62)
#define CNC_SELAXISU            (CNC_FIRST + 63)
#define CNC_SELAXISV            (CNC_FIRST + 64)
#define CNC_SELAXISW            (CNC_FIRST + 65)
#define CNC_JOGINDEX            (CNC_FIRST + 66)
#define CNC_SELJOGHANDLE        (CNC_FIRST + 67)
#define CNC_SELJOGINC           (CNC_FIRST + 68)
#define CNC_SELJOGCONT          (CNC_FIRST + 69)
#define CNC_SELJOGHOME          (CNC_FIRST + 70)
#define CNC_DISTINDEX           (CNC_FIRST + 71)
#define CNC_SELDISTX1           (CNC_FIRST + 72)
#define CNC_SELDISTX10          (CNC_FIRST + 73)
#define CNC_SELDISTX100         (CNC_FIRST + 74)
#define CNC_SELDISTX1000        (CNC_FIRST + 75)
#define CNC_SELDISTX10000       (CNC_FIRST + 76)
#define CNC_SPEEDINDEX          (CNC_FIRST + 77)
#define CNC_SELSPEEDX1          (CNC_FIRST + 78)
#define CNC_SELSPEEDX10         (CNC_FIRST + 79)
#define CNC_SELSPEEDX100        (CNC_FIRST + 80)
#define CNC_SELSPEEDX1000       (CNC_FIRST + 81)
#define CNC_SELSPEEDX10000      (CNC_FIRST + 82)
#define CNC_SPINDLEINDEX        (CNC_FIRST + 83)
#define CNC_SPINDLECW           (CNC_FIRST + 84)
#define CNC_SPINDLECCW          (CNC_FIRST + 85)
#define CNC_SPINDLEOFF          (CNC_FIRST + 86)
#define CNC_SPARE2				      (CNC_FIRST + 87)
#define CNC_SPINDLEOVR          (CNC_FIRST + 88)
#define CNC_SPARE3				      (CNC_FIRST + 89)
#define CNC_FEEDOVR             (CNC_FIRST + 90)
#define CNC_SPARE4				      (CNC_FIRST + 91)
#define CNC_RAPIDOVR            (CNC_FIRST + 92)
#define CNC_RESET               (CNC_FIRST + 93)
#define CNC_CYCLESTART          (CNC_FIRST + 94)
#define CNC_FEEDHOLD            (CNC_FIRST + 95)
#define CNC_REWINDPROG          (CNC_FIRST + 96)
#define CNC_SEARCH              (CNC_FIRST + 97)
#define CNC_SEARCHNEXT          (CNC_FIRST + 98)
#define CNC_CLEARERRORS         (CNC_FIRST + 99)
#define CNC_HOME                (CNC_FIRST + 100)
#define CNC_SELCOORD1           (CNC_FIRST + 101)
#define CNC_SELCOORD2           (CNC_FIRST + 102)
#define CNC_HOME_ALL            (CNC_FIRST + 103)
#define CNC_TURRETSTOP          (CNC_FIRST + 104)
#define CNC_TOOL_RELEASE        (CNC_FIRST + 105)
#define CNC_TOOL_ENGAGE         (CNC_FIRST + 106)
#define CNC_TOOL_TOGGLE         (CNC_FIRST + 107)
#define CNC_CHIP_CONV_TOGGLE    (CNC_FIRST + 108)
#define CNC_CHIP_CONV_ON        (CNC_FIRST + 109)
#define CNC_CHIP_CONV_OFF       (CNC_FIRST + 110)
#define CNC_SPINDLE_DECT_ON     (CNC_FIRST + 111)
#define CNC_SPINDLE_DECT_OFF    (CNC_FIRST + 112)
#define CNC_DRY_RUN             (CNC_FIRST + 113)
#define CNC_ORGIN_AXIS          (CNC_FIRST + 114)
#define CNC_ORGIN_ALL           (CNC_FIRST + 115)
#define CNC_ERRMSGBOX           (CNC_FIRST + 116)
#define CNC_ERRUPDATE           (CNC_FIRST + 117)
#define CNC_LAST                (CNC_FIRST + 118)
#define DU_NULL					(0x0L)
#define DU_TOOLS				(0x1L)
#define DU_OFS					(0x2L)
#define DU_MODE                 (0x3L)
#define PC_UB_CMD_BOX1                 0x3DA8
#define PC_UB_DATA_BOX1                0x3DAC
#define PC_UB_CMD_BOX2                 0x3DB0
#define PC_UB_DATA_BOX2                0x3DB4
#define PC_UB_CMD_BOX3                 0x3DB8
#define PC_UB_DATA_BOX3                0x3DBC
#define PC_UB_CMD_BOX4                 0x3DC0
#define PC_UB_DATA_BOX4                0x3DC4
#define PC_UB_CMD_BOX5                 0x3DC8
#define PC_UB_DATA_BOX5                0x3DCC
#define PC_UB_CMD_BOX6                 0x3DD0
#define PC_UB_DATA_BOX6                0x3DD4
#define PC_UB_CMD_BOX7                 0x3DD8
#define PC_UB_DATA_BOX7                0x3DDC


extern RPC_IF_HANDLE __MIDL_itf_PmacServer_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_PmacServer_0000_v0_0_s_ifspec;


#ifndef __PCOMMSERVERLib_LIBRARY_DEFINED__
#define __PCOMMSERVERLib_LIBRARY_DEFINED__

/* library PCOMMSERVERLib */
/* [helpstring][version][uuid] */ 

typedef /* [public] */ 
enum DEVPMACTYPE
    {	DEV_PT_PMAC1	= 1,
	DEV_PT_PMAC2	= DEV_PT_PMAC1 + 1,
	DEV_PT_PMACUL	= DEV_PT_PMAC2 + 1,
	DEV_PT_GEOPMAC	= DEV_PT_PMACUL + 1,
	DEV_PT_PMAC	= DEV_PT_GEOPMAC + 1,
	DEV_PT_PMAC1T	= DEV_PT_PMAC + 1,
	DEV_PT_PMAC2T	= DEV_PT_PMAC1T + 1,
	DEV_PT_PMACUT	= DEV_PT_PMAC2T + 1,
	DEV_PT_UMAC	= DEV_PT_PMACUT + 1,
	DEV_PT_QMAC	= DEV_PT_UMAC + 1,
	DEV_PT_PMAC1TSM	= DEV_PT_QMAC + 1,
	DEV_PT_PMAC2TSM	= DEV_PT_PMAC1TSM + 1
    } 	DEVPMACTYPE;

typedef /* [public] */ 
enum DEVPMACFIRMWARETYPE
    {	DEV_PID	= 1,
	DEV_ESA	= DEV_PID + 1,
	DEV_LHB	= DEV_ESA + 1
    } 	DEVPMACFIRMWARETYPE;

typedef /* [public] */ 
enum DEVLOCATIONTYPE
    {	DEV_LT_UNKNOWN	= 0,
	DEV_LT_ISA	= DEV_LT_UNKNOWN + 1,
	DEV_LT_SER	= DEV_LT_ISA + 1,
	DEV_LT_ETH	= DEV_LT_SER + 1,
	DEV_LT_PCI	= DEV_LT_ETH + 1,
	DEV_LT_USB	= DEV_LT_PCI + 1,
	DEV_LT_LAST	= DEV_LT_USB + 1
    } 	DEVLOCATIONTYPE;

typedef /* [public] */ 
enum DEVASCIIMODE
    {	DEV_BUS	= 0,
	DEV_DPR	= DEV_BUS + 1
    } 	DEVASCIIMODE;

typedef /* [uuid][public] */  DECLSPEC_UUID("393DA2FF-814B-4b76-A27A-9CD4E22DCA61") struct DEVVBGWFORMAT
    {
    long type_addr;
    long data1;
    long data2;
    } 	DEVVBGWFORMAT;

typedef /* [uuid][public] */  DECLSPEC_UUID("5B24D4EE-2ADF-47a0-9AE1-C44FC9F60D64") struct DEVBGVREADSTATUS
    {
    USHORT num_entries;
    USHORT num_data;
    USHORT addr_offset;
    USHORT data_offset;
    } 	DEVBGVREADSTATUS;

typedef /* [uuid][public] */  DECLSPEC_UUID("3AE1BF80-C9DB-421d-9810-F922C7F48353") struct DEVPLC_STATUS_STRUCT
    {
    BOOL plc_enabled;
    long plc_start_addr;
    long plc_end_addr;
    long plc_size;
    } 	DEVPLC_STATUS_STRUCT;

typedef /* [uuid][public] */  DECLSPEC_UUID("4285678F-0504-49c0-AD56-9E50DC6609EC") struct DEVPLCC_STATUS_STRUCT
    {
    BOOL plcc_loaded;
    BOOL plcc_enabled;
    long plcc_start_addr;
    long plcc_end_addr;
    long plcc_size;
    } 	DEVPLCC_STATUS_STRUCT;

typedef /* [uuid][public] */  DECLSPEC_UUID("886BD577-3E89-4888-A1B8-5D5B499B63EC") struct DEVTOTAL_PLC_STATUS_STRUCT
    {
    long num_plc;
    long num_plcc;
    long I5;
    DEVPLC_STATUS_STRUCT plc[ 32 ];
    DEVPLCC_STATUS_STRUCT plcc[ 32 ];
    } 	DEVTOTAL_PLC_STATUS_STRUCT;

typedef /* [public] */ 
enum DEVGATMODE
    {	DEVGAT_RAM_NOWRAP	= 0,
	DEVGAT_RAM_WRAP	= DEVGAT_RAM_NOWRAP + 1,
	DEVGAT_DPR_NOWRAP	= DEVGAT_RAM_WRAP + 1,
	DEVGAT_DPR_WRAP	= DEVGAT_DPR_NOWRAP + 1
    } 	DEVGATMODE;

typedef /* [uuid][public] */  DECLSPEC_UUID("4A568FA4-62CE-485d-B412-2A5D7CEFB59B") struct DEVWTG_EX
    {
    UINT DEVCOM_TO_G;
    UINT DEVENC_TO_G;
    UINT DEVDAC_TO_G;
    UINT DEVCUR_TO_G;
    } 	DEVWTG_EX;

typedef /* [uuid][public] */  DECLSPEC_UUID("8806F6C1-C9AD-400b-8AE0-01C8368FE081") struct DEVGATHER_INFO
    {
    VARIANT_BOOL bGatherEnabled;
    BSTR szGatherAdr;
    long lGatherSize;
    double dGatherScale;
    SAFEARRAY * saGatherData;
    } 	DEVGATHER_INFO;

typedef /* [uuid][public] */  DECLSPEC_UUID("8806F6C1-C9AE-400b-8AE0-01C8368FE081") struct DEVGATHER_HEADER
    {
    DWORD size;
    double ulGatherSampleTime;
    UINT uGatherPeriod;
    DWORD dwGatherMask;
    DWORD dwGatherMask2;
    UINT uGatherSources;
    UINT uGatherSamples;
    UINT uGatherSampleLen;
    BOOL bGatherEnabled[ 48 ];
    unsigned char szGatherAdr[ 48 ][ 15 ];
    UINT uGatherSize[ 48 ];
    double *pGatherData[ 48 ];
    double dGatherScale[ 48 ];
    } 	DEVGATHER_HEADER;

typedef /* [public] */ 
enum DEVFEEDBACKTYPE
    {	DEVFB_RESOLVER	= 0,
	DEVFB_ABSOLUTE	= DEVFB_RESOLVER + 1,
	DEVFB_UVW	= DEVFB_ABSOLUTE + 1,
	DEVFB_INCREMENTAL	= DEVFB_UVW + 1,
	DEVFB_MLDT	= DEVFB_INCREMENTAL + 1,
	DEVFB_ANALOG	= DEVFB_MLDT + 1
    } 	DEVFEEDBACKTYPE;

typedef /* [uuid][public] */  DECLSPEC_UUID("487269C0-F8D0-4427-964E-255062AB06AD") struct DEVULTRALIGHT_INFO
    {
    DWORD dwNumActiveNodes;
    DWORD dwNumMACROStations;
    DWORD dwNodeActive[ 64 ];
    DWORD dwIsIONode[ 64 ];
    DWORD dwMACROStationID[ 64 ];
    } 	DEVULTRALIGHT_INFO;

typedef /* [public] */ 
enum DEVPROGRAMMODE
    {	DEV_PRG_STOP	= 0,
	DEV_PRG_RUN	= DEV_PRG_STOP + 1,
	DEV_PRG_STEP	= DEV_PRG_RUN + 1,
	DEV_PRG_HOLD	= DEV_PRG_STEP + 1,
	DEV_PRG_JOGHOLD	= DEV_PRG_HOLD + 1,
	DEV_PRG_JOGSTOP	= DEV_PRG_JOGHOLD + 1
    } 	DEVPROGRAMMODE;

typedef /* [public] */ 
enum DEVMOTION
    {	DEV_MOTION_INPOS	= 0,
	DEV_MOTION_JOG	= DEV_MOTION_INPOS + 1,
	DEV_MOTION_RUNNING	= DEV_MOTION_JOG + 1,
	DEV_MOTION_HOMING	= DEV_MOTION_RUNNING + 1,
	DEV_MOTION_HANDLE	= DEV_MOTION_HOMING + 1,
	DEV_MOTION_OPENLOOP	= DEV_MOTION_HANDLE + 1,
	DEV_MOTION_DISABLED	= DEV_MOTION_OPENLOOP + 1
    } 	DEVMOTION;

typedef /* [public] */ 
enum DEVMOTIONMODE
    {	DEV_MOTIONMODE_LINEAR	= 0,
	DEV_MOTIONMODE_RAPID	= DEV_MOTIONMODE_LINEAR + 1,
	DEV_MOTIONMODE_CIRCW	= DEV_MOTIONMODE_RAPID + 1,
	DEV_MOTIONMODE_CIRCCW	= DEV_MOTIONMODE_CIRCW + 1,
	DEV_MOTIONMODE_SPLINE	= DEV_MOTIONMODE_CIRCCW + 1,
	DEV_MOTIONMODE_PVT	= DEV_MOTIONMODE_SPLINE + 1
    } 	DEVMOTIONMODE;

typedef /* [public] */ 
enum DEVDPRBINROTSTATUS
    {	DEV_DPRERROR	= -1,
	DEV_DPROK	= DEV_DPRERROR + 1,
	DEV_DPRBUFBSY	= DEV_DPROK + 1,
	DEV_DPREOF	= DEV_DPRBUFBSY + 1
    } 	DEVDPRBINROTSTATUS;

typedef /* [uuid][public] */  DECLSPEC_UUID("3EA804DC-232C-4034-8782-6C96193CB5DB") struct DEVFGMOTORTURBO
    {
    __int64 pepos;
    long pdac;
    long sstatus;
    long msstatus;
    __int64 bpos;
    long fvel;
    __int64 hwpos;
    __int64 pos;
    } 	DEVFGMOTORTURBO;

typedef /* [uuid][public] */  DECLSPEC_UUID("30D26998-DE15-4a48-98E9-D6775FB9CF07") struct DEVFGMOTOR
    {
    __int64 dpos;
    __int64 apos;
    __int64 hwpos;
    __int64 cpos;
    long pdac;
    long sstatus;
    long fvel;
    long movtim;
    long mspare[ 3 ];
    } 	DEVFGMOTOR;

typedef /* [uuid][public] */  DECLSPEC_UUID("AB7AA06E-496B-4b00-AE82-123EAFEFFA97") struct DEVBGCOORD
    {
    __int64 mpos;
    __int64 bpos;
    long mstatus;
    __int64 cstatus;
    __int64 cpos[ 9 ];
    long pstatus;
    long pr;
    long timrem;
    long tats;
    long pe;
    long fvel;
    } 	DEVBGCOORD;

typedef /* [uuid][public] */  DECLSPEC_UUID("AA40823A-421B-4356-AAE4-AED66BBC3823") struct DEVBGCOORDTURBO
    {
    __int64 ffdrate;
    long fdpot;
    short fdadr;
    short fdhld;
    long cstatus;
    __int64 cpos[ 9 ];
    long pstatus;
    long pr;
    long timrem;
    long tats;
    long pe;
    long spares[ 3 ];
    } 	DEVBGCOORDTURBO;

typedef /* [uuid][public] */  DECLSPEC_UUID("0F4295F0-D83B-4dea-A27E-60E8ADFBA6D6") struct DEVPOSDATASTRUCT
    {
    double servocount;
    double x;
    double y;
    double z;
    double xfe;
    double yfe;
    double zfe;
    double feMag;
    } 	DEVPOSDATASTRUCT;

typedef /* [uuid][public] */  DECLSPEC_UUID("A1670AD2-1F58-4a32-8BAD-3493184B2D27") struct DEVDATA
    {
    double servoTime;
    SAFEARRAY * commandedPosition;
    SAFEARRAY * commandedVelocity;
    SAFEARRAY * commandedAcceleration;
    SAFEARRAY * actualPosition;
    SAFEARRAY * actualAverageVelocity;
    SAFEARRAY * actualAcceleration;
    SAFEARRAY * followingError;
    SAFEARRAY * servoCommand;
    } 	DEVDATA;

typedef /* [uuid][public] */  DECLSPEC_UUID("6387F23B-2BCC-4865-A016-C9D6677B44DE") struct DEVPOSDATA
    {
    long tag_type;
    /* [switch_is] */ /* [switch_type] */ union __MIDL___MIDL_itf_PmacServer_0000_0001
        {
        /* [case()] */ double d[ 8 ];
        /* [case()] */ DEVPOSDATASTRUCT s;
        } 	U1;
    } 	DEVPOSDATA;

typedef /* [uuid][public] */  DECLSPEC_UUID("A1670AD2-1F57-4a32-8BAD-3493184B2D27") struct pmacStripPlotDATA
    {
    double servoTime;
    double commandedPosition[ 32 ];
    double commandedVelocity[ 32 ];
    double commandedAcceleration[ 32 ];
    double actualPosition[ 32 ];
    double actualAverageVelocity[ 32 ];
    double actualAcceleration[ 32 ];
    double followingError[ 32 ];
    double servoCommand[ 32 ];
    } 	pmacStripPlotDATA;

typedef /* [uuid][public] */  DECLSPEC_UUID("F655ABB9-D995-4f86-964B-CA37ADCE3B73") struct DEVMSTURBO
    {
    VARIANT_BOOL mst_motor_active;
    VARIANT_BOOL mst_neg_end_limit_set;
    VARIANT_BOOL mst_pos_end_limt_set;
    VARIANT_BOOL mst_ext_servo_algo_ena;
    VARIANT_BOOL mst_amplifier_enable;
    VARIANT_BOOL mst_open_loop_mode;
    VARIANT_BOOL mst_move_time_active;
    VARIANT_BOOL mst_integrate_mode;
    VARIANT_BOOL mst_dwell_in_progress;
    VARIANT_BOOL mst_data_block_error;
    VARIANT_BOOL mst_desired_velocity_0;
    VARIANT_BOOL mst_abort_decelerate_progs;
    VARIANT_BOOL mst_block_request;
    VARIANT_BOOL mst_home_search_progs;
    VARIANT_BOOL mst_user_written_phase_ena;
    VARIANT_BOOL mst_user_written_servo_ena;
    VARIANT_BOOL mst_y_addrs_commute;
    VARIANT_BOOL mst_commutation_enable;
    VARIANT_BOOL mst_pos_follow_offset_mode;
    VARIANT_BOOL mst_pos_follow_enable;
    VARIANT_BOOL mst_capture_error_anable;
    VARIANT_BOOL mst_software_capt_enable;
    VARIANT_BOOL mst_sign_magnitude_servo;
    VARIANT_BOOL mst_rapid_max_velocity;
    VARIANT_BOOL mst_cs_1_bit_3;
    VARIANT_BOOL mst_cs_1_bit_2;
    VARIANT_BOOL mst_cs_1_bit_1;
    VARIANT_BOOL mst_cs_1_bit_0;
    VARIANT_BOOL mst_cs_axis_def_bit_3;
    VARIANT_BOOL mst_cs_axis_def_bit_2;
    VARIANT_BOOL mst_cs_axis_def_bit_1;
    VARIANT_BOOL mst_cs_axis_def_bit_0;
    VARIANT_BOOL mst_assigned_to_cs;
    VARIANT_BOOL mst_reserver_for_future;
    VARIANT_BOOL mst_forground_in_position;
    VARIANT_BOOL mst_desired_position_stop;
    VARIANT_BOOL mst_stop_on_position_limit;
    VARIANT_BOOL mst_home_complete;
    VARIANT_BOOL mst_motor_phase_request;
    VARIANT_BOOL mst_phasing_search_error;
    VARIANT_BOOL mst_tigger_move;
    VARIANT_BOOL mst_integ_fatal_following_err;
    VARIANT_BOOL mst_i2t_amp_fault_error;
    VARIANT_BOOL mst_backlash_direction_flag;
    VARIANT_BOOL mst_amp_fault_error;
    VARIANT_BOOL mst_fault_error_exceeded;
    VARIANT_BOOL mst_warning_error_exceeded;
    VARIANT_BOOL mst_in_postion_true;
    } 	DEVMSTURBO;

typedef /* [uuid][public] */  DECLSPEC_UUID("D655ABB9-D095-4f96-964B-CA36ADCE3B73") struct DEVMSNONTURBO
    {
    VARIANT_BOOL msnt_motor_active;
    VARIANT_BOOL msnt_neg_end_limit_set;
    VARIANT_BOOL msnt_pos_end_limit_set;
    VARIANT_BOOL msnt_handwheel_enable;
    VARIANT_BOOL msnt_phased_motor;
    VARIANT_BOOL msnt_open_loop_mode;
    VARIANT_BOOL msnt_running_a_program;
    VARIANT_BOOL msnt_integrate_mode;
    VARIANT_BOOL msnt_dwell_in_progress;
    VARIANT_BOOL msnt_data_block_error;
    VARIANT_BOOL msnt_desired_velocity_0;
    VARIANT_BOOL msnt_abort_deceleration;
    VARIANT_BOOL msnt_block_request;
    VARIANT_BOOL msnt_home_search_active;
    VARIANT_BOOL msnt_insternal_x9;
    VARIANT_BOOL msnt_insternal_x8;
    VARIANT_BOOL msnt_insternal_x7;
    VARIANT_BOOL msnt_insternal_x6;
    VARIANT_BOOL msnt_insternal_x5;
    VARIANT_BOOL msnt_insternal_x4;
    VARIANT_BOOL msnt_insternal_x3;
    VARIANT_BOOL msnt_insternal_x2;
    VARIANT_BOOL msnt_insternal_x1;
    VARIANT_BOOL msnt_insternal_x0;
    VARIANT_BOOL msnt_assigned_to_cs;
    VARIANT_BOOL msnt_cs_1_bit_2;
    VARIANT_BOOL msnt_cs_1_bit_1;
    VARIANT_BOOL msnt_cs_1_bit_0;
    VARIANT_BOOL msnt_reserved_y19;
    VARIANT_BOOL msnt_reserved_y18;
    VARIANT_BOOL msnt_reserved_y17;
    VARIANT_BOOL msnt_reserved_y16;
    VARIANT_BOOL msnt_reserved_y15;
    VARIANT_BOOL msnt_amplifier_enabled;
    VARIANT_BOOL msnt_reserved_y13;
    VARIANT_BOOL msnt_reserved_y12;
    VARIANT_BOOL msnt_stop_on_position_limit;
    VARIANT_BOOL msnt_home_complete;
    VARIANT_BOOL msnt_reserved_y9;
    VARIANT_BOOL msnt_phasing_search_error;
    VARIANT_BOOL msnt_tigger_move;
    VARIANT_BOOL msnt_integ_fatal_follow_err;
    VARIANT_BOOL msnt_i2t_amp_fault_error;
    VARIANT_BOOL msnt_backlash_direction_flag;
    VARIANT_BOOL msnt_amp_fault_error;
    VARIANT_BOOL msnt_fault_following_error;
    VARIANT_BOOL msnt_warning_following_error;
    VARIANT_BOOL msnt_in_position_true;
    } 	DEVMSNONTURBO;

typedef /* [uuid][public] */  DECLSPEC_UUID("D666ABC9-D195-4f95-965B-CA36ADDE4B73") struct DEVGSTURBO
    {
    VARIANT_BOOL gst_main_error;
    VARIANT_BOOL gst_rti_reentry_error;
    VARIANT_BOOL gst_cpu_type_1;
    VARIANT_BOOL gst_servo_error;
    VARIANT_BOOL gst_data_gath_enabled;
    VARIANT_BOOL gst_reserved_x18;
    VARIANT_BOOL gst_gather_external_trig;
    VARIANT_BOOL gst_small_mem_turbo_pmac;
    VARIANT_BOOL gst_internal_15;
    VARIANT_BOOL gst_compensate_table_on;
    VARIANT_BOOL gst_general_checksum_err;
    VARIANT_BOOL gst_firmware_checksum_err;
    VARIANT_BOOL gst_dpram_error;
    VARIANT_BOOL gst_earom_error;
    VARIANT_BOOL gst_real_time_interr_warn;
    VARIANT_BOOL gst_illeagal_l_var_def;
    VARIANT_BOOL gst_servo_macro_ic_config_err;
    VARIANT_BOOL gst_tws_var_partity_error;
    VARIANT_BOOL gst_macro_comm_error;
    VARIANT_BOOL gst_macro_ring_error;
    VARIANT_BOOL gst_no_phase_clock_error;
    VARIANT_BOOL gst_reserved_x2;
    VARIANT_BOOL gst_all_cards_add_serially;
    VARIANT_BOOL gst_this_cards_add_serially;
    VARIANT_BOOL gst_turbo_ultralite;
    VARIANT_BOOL gst_turbo_vme;
    VARIANT_BOOL gst_cpu_type;
    VARIANT_BOOL gst_binary_rotary_buff_open;
    VARIANT_BOOL gst_motion_buffer_open;
    VARIANT_BOOL gst_ascii_rotary_buffer_open;
    VARIANT_BOOL gst_plc_buffer_open;
    VARIANT_BOOL gst_umac_turbo;
    VARIANT_BOOL gst_internal_y15;
    VARIANT_BOOL gst_internal_y14;
    VARIANT_BOOL gst_reserved_y13;
    VARIANT_BOOL gst_reserved_y12;
    VARIANT_BOOL gst_fixed_buffer_full;
    VARIANT_BOOL gst_macro_ring_test_enable;
    VARIANT_BOOL gst_ring_active;
    VARIANT_BOOL gst_modbus_active;
    VARIANT_BOOL gst_reserved_y7;
    VARIANT_BOOL gst_reserved_y6;
    VARIANT_BOOL gst_macro_ring_rcvd_break_msg;
    VARIANT_BOOL gst_macro_ring_break;
    VARIANT_BOOL gst_macro_ring_syn_pack_fault;
    VARIANT_BOOL gst_reserved_y2;
    VARIANT_BOOL gst_reserved_y1;
    VARIANT_BOOL gst_e_stop;
    } 	DEVGSTURBO;

typedef /* [uuid][public] */  DECLSPEC_UUID("D656ABB9-D195-4f95-965B-CA36ADCE3B73") struct DEVGSNONTURBO
    {
    VARIANT_BOOL gsnt_real_time_interr_active;
    VARIANT_BOOL gsnt_real_time_interr_reentry;
    VARIANT_BOOL gsnt_servo_active;
    VARIANT_BOOL gsnt_servo_error;
    VARIANT_BOOL gsnt_data_gather_enabled;
    VARIANT_BOOL gsnt_gather_on_next_servo;
    VARIANT_BOOL gsnt_gather_on_enternal_trig;
    VARIANT_BOOL gsnt_reserved_x16;
    VARIANT_BOOL gsnt_reserved_x15;
    VARIANT_BOOL gsnt_compensate_table_open;
    VARIANT_BOOL gsnt_general_checksum_err;
    VARIANT_BOOL gsnt_firmware_checksum_err;
    VARIANT_BOOL gsnt_dpram_error;
    VARIANT_BOOL gsnt_earom_error;
    VARIANT_BOOL gsnt_internal_x9;
    VARIANT_BOOL gsnt_internal_x8;
    VARIANT_BOOL gsnt_internal_x7;
    VARIANT_BOOL gsnt_tws_var_parity_error;
    VARIANT_BOOL gsnt_marco_aux_error;
    VARIANT_BOOL gsnt_reserved_x4;
    VARIANT_BOOL gsnt_reserved_x3;
    VARIANT_BOOL gsnt_reserved_x2;
    VARIANT_BOOL gsnt_all_cards_add_serially;
    VARIANT_BOOL gsnt_this_cards_add_serially;
    VARIANT_BOOL gsnt_extended_read;
    VARIANT_BOOL gsnt_host_port_comm_mode;
    VARIANT_BOOL gsnt_internal_y21;
    VARIANT_BOOL gsnt_internal_y20;
    VARIANT_BOOL gsnt_motion_buffer_open;
    VARIANT_BOOL gsnt_rotary_buffer_open;
    VARIANT_BOOL gsnt_plc_buffer_open;
    VARIANT_BOOL gsnt_plc_command;
    VARIANT_BOOL gsnt_vme_port_comm_mode;
    VARIANT_BOOL gsnt_internal_y14;
    VARIANT_BOOL gsnt_internal_y13;
    VARIANT_BOOL gsnt_internal_y12;
    VARIANT_BOOL gsnt_fixed_buffer_full;
    VARIANT_BOOL gsnt_internal_y10;
    VARIANT_BOOL gsnt_internal_y9;
    VARIANT_BOOL gsnt_internal_y8;
    VARIANT_BOOL gsnt_reserved_y7;
    VARIANT_BOOL gsnt_reserved_y6;
    VARIANT_BOOL gsnt_reserved_y5;
    VARIANT_BOOL gsnt_reserved_y4;
    VARIANT_BOOL gsnt_reserved_y3;
    VARIANT_BOOL gsnt_reserved_y2;
    VARIANT_BOOL gsnt_reserved_y1;
    VARIANT_BOOL gsnt_reserved_y0;
    } 	DEVGSNONTURBO;

typedef /* [uuid][public] */  DECLSPEC_UUID("D353ABB9-D135-4f35-965B-CA36ADCE3B33") struct DEVCSTURBO
    {
    VARIANT_BOOL cst_z_axis_used_in_feedrate;
    VARIANT_BOOL cst_z_axis_increment_mode;
    VARIANT_BOOL cst_y_axis_used_in_feedrate;
    VARIANT_BOOL cst_y_axis_increment_mode;
    VARIANT_BOOL cst_x_axis_used_in_feedrate;
    VARIANT_BOOL cst_x_axis_increment_mode;
    VARIANT_BOOL cst_w_axis_used_in_feedrate;
    VARIANT_BOOL cst_w_axis_increment_mode;
    VARIANT_BOOL cst_v_axis_used_in_feedrate;
    VARIANT_BOOL cst_v_axis_increment_mode;
    VARIANT_BOOL cst_u_axis_used_in_feedrate;
    VARIANT_BOOL cst_u_axis_increment_mode;
    VARIANT_BOOL cst_c_axis_used_in_feedrate;
    VARIANT_BOOL cst_c_axis_increment_mode;
    VARIANT_BOOL cst_b_axis_used_in_feedrate;
    VARIANT_BOOL cst_b_axis_increment_mode;
    VARIANT_BOOL cst_a_axis_used_in_feedrate;
    VARIANT_BOOL cst_a_axis_increment_mode;
    VARIANT_BOOL cst_radius_vec_incr_mode;
    VARIANT_BOOL cst_continuous_motion_req;
    VARIANT_BOOL cst_move_spec_by_time;
    VARIANT_BOOL cst_continous_motion_mode;
    VARIANT_BOOL cst_single_step_mode;
    VARIANT_BOOL cst_running_program;
    VARIANT_BOOL cst_lookahead_in_progres;
    VARIANT_BOOL cst_run_time_error;
    VARIANT_BOOL cst_internal_move_in_stack;
    VARIANT_BOOL cst_amp_fault_error;
    VARIANT_BOOL cst_fatal_following_error;
    VARIANT_BOOL cst_waring_follwoing_error;
    VARIANT_BOOL cst_in_position;
    VARIANT_BOOL cst_rotary_buffer_full;
    VARIANT_BOOL cst_delayed_calc_flag;
    VARIANT_BOOL cst_end_of_block_stop_progs;
    VARIANT_BOOL cst_intnal_sync_m_var_one_shot;
    VARIANT_BOOL cst_intnal_dwell_move_buffered;
    VARIANT_BOOL cst_cttr_comp_outside_corner;
    VARIANT_BOOL cst_cttr_comp_stop_req;
    VARIANT_BOOL cst_cttr_comp_move_buffered;
    VARIANT_BOOL cst_pre_jog_move_in_progs;
    VARIANT_BOOL cst_segmented_move_in_progs;
    VARIANT_BOOL cst_segment_accel_flag;
    VARIANT_BOOL cst_segment_stop_request;
    VARIANT_BOOL cst_pvt_spline_move_mode;
    VARIANT_BOOL cst_cutter_comp_left;
    VARIANT_BOOL cst_cutter_comp_on;
    VARIANT_BOOL cst_ccw_circle_rapid_move_mode;
    VARIANT_BOOL cst_circle_spline_move_mode;
    VARIANT_BOOL cst_lhb_wrap;
    VARIANT_BOOL cst_internal_y22;
    VARIANT_BOOL cst_internal_y21;
    VARIANT_BOOL cst_internal_y20;
    VARIANT_BOOL cst_lhb_sync_m_var_overflow;
    VARIANT_BOOL cst_lhb_direction;
    VARIANT_BOOL cst_lhb_stop;
    VARIANT_BOOL cst_lhb_change;
    VARIANT_BOOL cst_lhb_last_segment;
    VARIANT_BOOL cst_lhb_recalculate;
    VARIANT_BOOL cst_lhb_flush;
    VARIANT_BOOL cst_lhb_lasr_move;
    VARIANT_BOOL cst_lhb_single_segment_request;
    VARIANT_BOOL cst_lhb_change_request;
    VARIANT_BOOL cst_lhb_move_request;
    VARIANT_BOOL cst_lhb_direction_request;
    VARIANT_BOOL cst_reserved_y7;
    VARIANT_BOOL cst_reserved_y6;
    VARIANT_BOOL cst_reserved_y5;
    VARIANT_BOOL cst_reserved_y4;
    VARIANT_BOOL cst_radius_error;
    VARIANT_BOOL cst_prog_resume_error;
    VARIANT_BOOL cst_desired_pos_limit_stop;
    VARIANT_BOOL cst_in_prog_pmatch;
    } 	DEVCSTURBO;

typedef /* [uuid][public] */  DECLSPEC_UUID("D344ABB9-D435-4f35-965B-CA46ADCE3B44") struct DEVCSNONTURBO
    {
    VARIANT_BOOL csnt_z_axis_used_in_feedrate;
    VARIANT_BOOL csnt_z_axis_increment_mode;
    VARIANT_BOOL csnt_y_axis_used_in_feedrate;
    VARIANT_BOOL csnt_y_axis_increment_mode;
    VARIANT_BOOL csnt_x_axis_used_in_feedrate;
    VARIANT_BOOL csnt_x_axis_increment_mode;
    VARIANT_BOOL csnt_w_axis_used_in_feedrate;
    VARIANT_BOOL csnt_w_axis_increment_mode;
    VARIANT_BOOL csnt_v_axis_used_in_feedrate;
    VARIANT_BOOL csnt_v_axis_increment_mode;
    VARIANT_BOOL csnt_u_axis_used_in_feedrate;
    VARIANT_BOOL csnt_u_axis_increment_mode;
    VARIANT_BOOL csnt_c_axis_used_in_feedrate;
    VARIANT_BOOL csnt_c_axis_increment_mode;
    VARIANT_BOOL csnt_b_axis_used_in_feedrate;
    VARIANT_BOOL csnt_b_axis_increment_mode;
    VARIANT_BOOL csnt_a_axis_used_in_feedrate;
    VARIANT_BOOL csnt_a_axis_increment_mode;
    VARIANT_BOOL csnt_radius_vec_incr_mode;
    VARIANT_BOOL csnt_continuous_motion_req;
    VARIANT_BOOL csnt_move_spec_by_time;
    VARIANT_BOOL csnt_continous_motion_mode;
    VARIANT_BOOL csnt_single_step_mode;
    VARIANT_BOOL csnt_running_program;
    VARIANT_BOOL csnt_program_hold_in_progress;
    VARIANT_BOOL csnt_run_time_error;
    VARIANT_BOOL csnt_circle_radius_error;
    VARIANT_BOOL csnt_amp_fault_error;
    VARIANT_BOOL csnt_fatal_following_err;
    VARIANT_BOOL csnt_waring_follwoing_err;
    VARIANT_BOOL csnt_in_position;
    VARIANT_BOOL csnt_rotary_buffer_full;
    VARIANT_BOOL csnt_delayed_calc_flag;
    VARIANT_BOOL csnt_end_of_block_stop_progs;
    VARIANT_BOOL csnt_intnal_sync_m_var_one_shot;
    VARIANT_BOOL csnt_intnal_dwell_move_buffered;
    VARIANT_BOOL csnt_cttr_comp_outside_corner;
    VARIANT_BOOL csnt_cttr_comp_stop_req;
    VARIANT_BOOL csnt_cttr_comp_move_buffered;
    VARIANT_BOOL csnt_pre_jog_move_in_progs;
    VARIANT_BOOL csnt_segmented_move;
    VARIANT_BOOL csnt_segment_accel_flag;
    VARIANT_BOOL csnt_segment_stop_request;
    VARIANT_BOOL csnt_pvt_spline_mode;
    VARIANT_BOOL csnt_cutter_comp_left;
    VARIANT_BOOL csnt_cutter_comp_on;
    VARIANT_BOOL csnt_ccw_circle_rapid_move_mode;
    VARIANT_BOOL csnt_circle_spline_move_mode;
    } 	DEVCSNONTURBO;

typedef /* [public] */ 
enum NCPOSTYPE
    {	NC_ABSOLUTE	= 0,
	NC_RELATIVE	= NC_ABSOLUTE + 1,
	NC_MACHINE	= NC_RELATIVE + 1,
	NC_COMMANDED	= NC_MACHINE + 1,
	NC_DISTTOGO	= NC_COMMANDED + 1,
	NC_FERR	= NC_DISTTOGO + 1
    } 	NCPOSTYPE;

typedef /* [public] */ 
enum NCMODE
    {	NC_MODE_UNKNOWN	= 0,
	NC_MODE_AUTO	= NC_MODE_UNKNOWN + 1,
	NC_MODE_MANUAL	= NC_MODE_AUTO + 1,
	NC_MODE_MDI	= NC_MODE_MANUAL + 1,
	NC_MODE_LAST	= NC_MODE_MDI + 1
    } 	NCMODE;

typedef /* [public] */ 
enum NCAXISSELECT
    {	NC_SELECT_UNKNOWN	= 0,
	NC_SELECTX	= NC_SELECT_UNKNOWN + 1,
	NC_SELECTY	= NC_SELECTX + 1,
	NC_SELECTZ	= NC_SELECTY + 1,
	NC_SELECTA	= NC_SELECTZ + 1,
	NC_SELECTB	= NC_SELECTA + 1,
	NC_SELECTC	= NC_SELECTB + 1,
	NC_SELECTU	= NC_SELECTC + 1,
	NC_SELECTV	= NC_SELECTU + 1,
	NC_SELECTW	= NC_SELECTV + 1,
	NC_SELECTLAST	= NC_SELECTW + 1
    } 	NCAXISSELECT;

typedef /* [public] */ 
enum NCJOGSELECT
    {	NC_JOG_UNKNOWN	= 0,
	NC_JOG_HANDWHEEL	= NC_JOG_UNKNOWN + 1,
	NC_JOG_INCREMENTAL	= NC_JOG_HANDWHEEL + 1,
	NC_JOG_CONTINUOUS	= NC_JOG_INCREMENTAL + 1,
	NC_JOG_HOME	= NC_JOG_CONTINUOUS + 1,
	NC_JOG_LAST	= NC_JOG_HOME + 1
    } 	NCJOGSELECT;

typedef /* [public] */ 
enum NCSPEEDSELECT
    {	NC_SPEED_UNKNOWN	= 0,
	NC_SPEED_LOW	= NC_SPEED_UNKNOWN + 1,
	NC_SPEED_MEDL	= NC_SPEED_LOW + 1,
	NC_SPEED_MEDIUM	= NC_SPEED_MEDL + 1,
	NC_SPEED_MEDH	= NC_SPEED_MEDIUM + 1,
	NC_SPEED_HIGH	= NC_SPEED_MEDH + 1,
	NC_SPEED_LAST	= NC_SPEED_HIGH + 1
    } 	NCSPEEDSELECT;

typedef /* [public] */ 
enum NCSPINDLESELECT
    {	NC_SPINDLE_UNKNOWN	= 0,
	NC_SPINDLE_CCW	= NC_SPINDLE_UNKNOWN + 1,
	NC_SPINDLE_OFF	= NC_SPINDLE_CCW + 1,
	NC_SPINDLE_CW	= NC_SPINDLE_OFF + 1,
	NC_SPINDLE_ORIENT1	= NC_SPINDLE_CW + 1,
	NC_SPINDLE_ORIENT2	= NC_SPINDLE_ORIENT1 + 1,
	NC_SPINDLE_LAST	= NC_SPINDLE_ORIENT2 + 1
    } 	NCSPINDLESELECT;

typedef /* [public] */ 
enum NCCLNTSELECT
    {	NC_COOLANT_UNKNOWN	= 0,
	NC_COOLANT_AUTO	= NC_COOLANT_UNKNOWN + 1,
	NC_COOLANT_FLOOD	= NC_COOLANT_AUTO + 1,
	NC_COOLANT_MIST	= NC_COOLANT_FLOOD + 1,
	NC_COOLANT_BOTH	= NC_COOLANT_MIST + 1,
	NC_COOLANT_LAST	= NC_COOLANT_BOTH + 1
    } 	NCCLNTSELECT;

typedef /* [public] */ 
enum NCBUFFERMODE
    {	NC_PARSEDTOBINARY	= 0,
	NC_FILETOBINARY	= NC_PARSEDTOBINARY + 1,
	NC_RESETBUFFER	= NC_FILETOBINARY + 1
    } 	NCBUFFERMODE;

typedef /* [public] */ 
enum NCTOOLMODE
    {	NC_TOOL_UNKNOWN	= 0,
	NC_TOOL_CHANGE	= NC_TOOL_UNKNOWN + 1,
	NC_TOOL_CAROUSELINC	= NC_TOOL_CHANGE + 1,
	NC_TOOL_CAROUSELDEC	= NC_TOOL_CAROUSELINC + 1,
	NC_TOOL_RELEASE	= NC_TOOL_CAROUSELDEC + 1,
	NC_TOOL_ENGAGE	= NC_TOOL_RELEASE + 1,
	NC_TOOL_LAST	= NC_TOOL_ENGAGE + 1
    } 	NCTOOLMODE;

typedef /* [public] */ 
enum NCERRORMODE
    {	NC_ERR_MESSAGE	= 0,
	NC_ERR_WARN	= NC_ERR_MESSAGE + 1,
	NC_ERR_STOP	= NC_ERR_WARN + 1,
	NC_ERR_FATAL	= NC_ERR_STOP + 1
    } 	NCERRORMODE;

typedef /* [public] */ 
enum NCERRORTYPE
    {	NC_ERR_SYSTEM	= 0,
	NC_ERR_MOTOR	= NC_ERR_SYSTEM + 1,
	NC_ERR_CODE	= NC_ERR_MOTOR + 1,
	NC_ERR_COMMON	= NC_ERR_CODE + 1,
	NC_ERR_PLC	= NC_ERR_COMMON + 1,
	NC_ERR_OPERMSG	= NC_ERR_PLC + 1
    } 	NCERRORTYPE;

typedef /* [public] */ 
enum NCTIMEBASEMODE
    {	NC_TIMEBASE_CUT	= 0,
	NC_TIMEBASE_RAPID	= NC_TIMEBASE_CUT + 1,
	NC_TIMEBASE_THREAD	= NC_TIMEBASE_RAPID + 1,
	NC_TIMEBASE_FPR	= NC_TIMEBASE_THREAD + 1,
	NC_TIMEBASE_LAST	= NC_TIMEBASE_FPR + 1
    } 	NCTIMEBASEMODE;

typedef /* [public] */ 
enum NCFEEDRATEMODE
    {	NC_FEEDRATE_FPM	= 0,
	NC_FEEDRATE_INVERSE	= NC_FEEDRATE_FPM + 1,
	NC_FEEDRATE_LAST	= NC_FEEDRATE_INVERSE + 1
    } 	NCFEEDRATEMODE;

typedef /* [public] */ 
enum NCFIVEAXISMODE
    {	NC_FIVEAXIS_DISABLED	= 0,
	NC_FIVEAXIS_AC	= NC_FIVEAXIS_DISABLED + 1,
	NC_FIVEAXIS_BC	= NC_FIVEAXIS_AC + 1,
	NC_FIVEAXIS_AB	= NC_FIVEAXIS_BC + 1,
	NC_FIVEAXIS_A	= NC_FIVEAXIS_AB + 1,
	NC_FIVEAXIS_B	= NC_FIVEAXIS_A + 1,
	NC_FIVEAXIS_C	= NC_FIVEAXIS_B + 1
    } 	NCFIVEAXISMODE;

typedef /* [public] */ 
enum NCMACH_STATE_TYPE
    {	NC_MCH_STATE_MANUAL	= 0,
	NC_MCH_STATE_MDI	= NC_MCH_STATE_MANUAL + 1,
	NC_MCH_STATE_AUTO	= NC_MCH_STATE_MDI + 1
    } 	NCMACH_STATE_TYPE;

typedef /* [public] */ 
enum NCMACHINETYPE
    {	NC_UNKNOWN	= 0,
	NC_MILL	= NC_UNKNOWN + 1,
	NC_LATHE	= NC_MILL + 1,
	NC_GRINDER	= NC_LATHE + 1,
	NC_LASER	= NC_GRINDER + 1,
	NC_EDM	= NC_LASER + 1
    } 	NCMACHINETYPE;

typedef /* [public] */ 
enum NCADDRESS
    {	NC_ADR_A	= 0,
	NC_ADR_B	= NC_ADR_A + 1,
	NC_ADR_C	= NC_ADR_B + 1,
	NC_ADR_X	= NC_ADR_C + 1,
	NC_ADR_Y	= NC_ADR_X + 1,
	NC_ADR_Z	= NC_ADR_Y + 1,
	NC_ADR_U	= NC_ADR_Z + 1,
	NC_ADR_V	= NC_ADR_U + 1,
	NC_ADR_W	= NC_ADR_V + 1,
	NC_ADR_E	= NC_ADR_W + 1,
	NC_ADR_F	= NC_ADR_E + 1,
	NC_ADR_G	= NC_ADR_F + 1,
	NC_ADR_H	= NC_ADR_G + 1,
	NC_ADR_D	= NC_ADR_H + 1,
	NC_ADR_I	= NC_ADR_D + 1,
	NC_ADR_J	= NC_ADR_I + 1,
	NC_ADR_K	= NC_ADR_J + 1,
	NC_ADR_L	= NC_ADR_K + 1,
	NC_ADR_P	= NC_ADR_L + 1,
	NC_ADR_Q	= NC_ADR_P + 1,
	NC_ADR_M	= NC_ADR_Q + 1,
	NC_ADR_N	= NC_ADR_M + 1,
	NC_ADR_O	= NC_ADR_N + 1,
	NC_ADR_R	= NC_ADR_O + 1,
	NC_ADR_S	= NC_ADR_R + 1,
	NC_ADR_T	= NC_ADR_S + 1,
	NC_ADR_UNKNOWN	= NC_ADR_T + 1
    } 	NCADDRESS;

typedef /* [public] */ 
enum NCTOOLTYPE
    {	NC_TOOLTYPE_UNKNOWN	= 0,
	NC_TOOLTYPE_ENDMILL	= NC_TOOLTYPE_UNKNOWN + 1,
	NC_TOOLTYPE_BALLMILL	= NC_TOOLTYPE_ENDMILL + 1,
	NC_TOOLTYPE_DRILL	= NC_TOOLTYPE_BALLMILL + 1,
	NC_TOOLTYPE_CSINK	= NC_TOOLTYPE_DRILL + 1,
	NC_TOOLTYPE_CDRILL	= NC_TOOLTYPE_CSINK + 1,
	NC_TOOLTYPE_INSERTMILL	= NC_TOOLTYPE_CDRILL + 1,
	NC_TOOLTYPE_LATHEINSERT	= NC_TOOLTYPE_INSERTMILL + 1,
	NC_TOOLTYPE_LAST_TOOL_TYPE	= NC_TOOLTYPE_LATHEINSERT + 1
    } 	NCTOOLTYPE;

typedef /* [public] */ 
enum NCMATERIALTYPE
    {	NC_MATERIAL_UNKNOWN	= 0,
	NC_MATERIAL_HSS	= NC_MATERIAL_UNKNOWN + 1,
	NC_MATERIAL_CARBIDE	= NC_MATERIAL_HSS + 1,
	NC_MATERIAL_LAST_MATERIAL_TYPE	= NC_MATERIAL_CARBIDE + 1
    } 	NCMATERIALTYPE;

typedef /* [public] */ 
enum NCTOOLHAND
    {	NC_TOOLHAND_Neutral	= 0,
	NC_TOOLHAND_Left	= NC_TOOLHAND_Neutral + 1,
	NC_TOOLHAND_Right	= NC_TOOLHAND_Left + 1
    } 	NCTOOLHAND;

typedef /* [public] */ 
enum NCTOOLDIRECTION
    {	NC_TOOLDIRECTION_NEUTRAL	= 0,
	NC_TOOLDIRECTION_45	= NC_TOOLDIRECTION_NEUTRAL + 1,
	NC_TOOLDIRECTION_135	= NC_TOOLDIRECTION_45 + 1,
	NC_TOOLDIRECTION_225	= NC_TOOLDIRECTION_135 + 1,
	NC_TOOLDIRECTION_315	= NC_TOOLDIRECTION_225 + 1,
	NC_TOOLDIRECTION_0	= NC_TOOLDIRECTION_315 + 1,
	NC_TOOLDIRECTION_90	= NC_TOOLDIRECTION_0 + 1,
	NC_TOOLDIRECTION_180	= NC_TOOLDIRECTION_90 + 1,
	NC_TOOLDIRECTION_270	= NC_TOOLDIRECTION_180 + 1,
	NC_TOOLDIRECTION_END	= NC_TOOLDIRECTION_270 + 1
    } 	NCTOOLDIRECTION;

typedef /* [public] */ 
enum NCTOOLOFSTYPE
    {	NC_TOOLOFSTYPE_GEOMETRY	= 0,
	NC_TOOLOFSTYPE_WEAR	= NC_TOOLOFSTYPE_GEOMETRY + 1,
	NC_TOOLOFSTYPE_LENGTH	= NC_TOOLOFSTYPE_WEAR + 1,
	NC_TOOLOFSTYPE_DIAMETER	= NC_TOOLOFSTYPE_LENGTH + 1,
	NC_TOOLOFSTYPE_ORIENTATION	= NC_TOOLOFSTYPE_DIAMETER + 1,
	NC_TOOLOFSTYPE_CUTTERCOMPENSATION	= NC_TOOLOFSTYPE_ORIENTATION + 1,
	NC_TOOLOFSTYPE_POCKET	= NC_TOOLOFSTYPE_CUTTERCOMPENSATION + 1,
	NC_TOOLOFSTYPE_PIVOTLENGTH	= NC_TOOLOFSTYPE_POCKET + 1,
	NC_TOOLOFSTYPE_CUTTERCOMPWEAR	= NC_TOOLOFSTYPE_PIVOTLENGTH + 1,
	NC_TOOLOFSTYPE_GROUPFAMILY	= NC_TOOLOFSTYPE_CUTTERCOMPWEAR + 1,
	NC_TOOLOFSTYPE_LIFECOUNT	= NC_TOOLOFSTYPE_GROUPFAMILY + 1,
	NC_TOOLOFSTYPE_LIFETIME	= NC_TOOLOFSTYPE_LIFECOUNT + 1
    } 	NCTOOLOFSTYPE;

typedef /* [public] */ 
enum NCCOORDTYPE
    {	NC_COORDTYPE_G54	= 0,
	NC_COORDTYPE_G55	= NC_COORDTYPE_G54 + 1,
	NC_COORDTYPE_G56	= NC_COORDTYPE_G55 + 1,
	NC_COORDTYPE_G57	= NC_COORDTYPE_G56 + 1,
	NC_COORDTYPE_G58	= NC_COORDTYPE_G57 + 1,
	NC_COORDTYPE_G59	= NC_COORDTYPE_G58 + 1,
	NC_COORDTYPE_G92	= NC_COORDTYPE_G59 + 1,
	NC_COORDTYPE_COMMON	= NC_COORDTYPE_G92 + 1,
	NC_COORDTYPE_TOOL	= NC_COORDTYPE_COMMON + 1,
	NC_COORDTYPE_INTERM	= NC_COORDTYPE_TOOL + 1,
	NC_COORDTYPE_SCALE	= NC_COORDTYPE_INTERM + 1,
	NC_COORDTYPE_MIRROR	= NC_COORDTYPE_SCALE + 1,
	NC_COORDTYPE_ROTATION	= NC_COORDTYPE_MIRROR + 1,
	NC_COORDTYPE_CENTER_ROT	= NC_COORDTYPE_ROTATION + 1,
	NC_COORDTYPE_G54_LOCAL	= NC_COORDTYPE_CENTER_ROT + 1,
	NC_COORDTYPE_G55_LOCAL	= NC_COORDTYPE_G54_LOCAL + 1,
	NC_COORDTYPE_G56_LOCAL	= NC_COORDTYPE_G55_LOCAL + 1,
	NC_COORDTYPE_G57_LOCAL	= NC_COORDTYPE_G56_LOCAL + 1,
	NC_COORDTYPE_G58_LOCAL	= NC_COORDTYPE_G57_LOCAL + 1,
	NC_COORDTYPE_G59_LOCAL	= NC_COORDTYPE_G58_LOCAL + 1,
	NC_COORDTYPE_PRODUCT	= NC_COORDTYPE_G59_LOCAL + 1,
	NC_COORDTYPE_OFFSET_REL	= NC_COORDTYPE_PRODUCT + 1,
	NC_COORDTYPE_OFFSET_ABS	= NC_COORDTYPE_OFFSET_REL + 1,
	NC_COORDTYPE_CENTER_SCALE	= NC_COORDTYPE_OFFSET_ABS + 1,
	NC_COORDTYPE_G53P1	= NC_COORDTYPE_CENTER_SCALE + 1,
	NC_COORDTYPE_G54P1	= NC_COORDTYPE_G53P1 + 1
    } 	NCCOORDTYPE;

typedef /* [uuid][public] */  DECLSPEC_UUID("DC1E4F4D-460F-4BB6-AFB6-6349ED9F7A52") struct NCTOOL
    {
    long dwControl;
    long nCoord;
    NCTOOLTYPE toolType;
    NCMATERIALTYPE toolMaterial;
    BOOL metricUnits;
    NCTOOLHAND hand;
    NCTOOLDIRECTION direction;
    long nToolNumber;
    long nPot;
    double diameter;
    double radius;
    double length;
    double pivotlength;
    double tipangle;
    double clrangle;
    double cornerrad;
    BOOL diacomp;
    double comp;
    double compwear;
    double ofsx;
    double ofsy;
    double ofsz;
    double ofsu;
    double ofsv;
    double ofsw;
    double ofsa;
    double ofsb;
    double ofsc;
    double ofs2x;
    double ofs2y;
    double ofs2z;
    double ofs2u;
    double ofs2v;
    double ofs2w;
    double gx;
    double gy;
    double gz;
    double turretX;
    double turretZ;
    long groupfamily;
    double lifecount;
    double lifetime;
    } 	NCTOOL;

typedef /* [uuid][public] */  DECLSPEC_UUID("BF167284-5EBC-4b7c-AA59-5B312E8E5C7C") struct NCTOOLLIFE
    {
    UINT toolNo;
    UINT groupfamily;
    double lifecount;
    double lifetime;
    } 	NCTOOLLIFE;

typedef /* [uuid][public] */  DECLSPEC_UUID("5B1330B2-D46E-4f0c-912D-2BB81BA76C20") struct NCDATA
    {
    double ServoTime;
    double X_Absolute;
    double Y_Absolute;
    double Z_Absolute;
    double A_Absolute;
    double B_Absolute;
    double C_Absolute;
    double U_Absolute;
    double V_Absolute;
    double W_Absolute;
    double X_Relative;
    double Y_Relative;
    double Z_Relative;
    double A_Relative;
    double B_Relative;
    double C_Relative;
    double U_Relative;
    double V_Relative;
    double W_Relative;
    double X_Machine;
    double Y_Machine;
    double Z_Machine;
    double A_Machine;
    double B_Machine;
    double C_Machine;
    double U_Machine;
    double V_Machine;
    double W_Machine;
    double X_Program;
    double Y_Program;
    double Z_Program;
    double A_Program;
    double B_Program;
    double C_Program;
    double U_Program;
    double V_Program;
    double W_Program;
    double X_DistToGo;
    double Y_DistToGo;
    double Z_DistToGo;
    double A_DistToGo;
    double B_DistToGo;
    double C_DistToGo;
    double U_DistToGo;
    double V_DistToGo;
    double W_DistToGo;
    double X_FError;
    double Y_FError;
    double Z_FError;
    double A_FError;
    double B_FError;
    double C_FError;
    double U_FError;
    double V_FError;
    double W_FError;
    VARIANT_BOOL InPosition;
    double FeedrateCommanded;
    double FeedrateActual;
    double FeedrateOverride;
    double RapidOverride;
    double SpindleCommanded;
    double SpindleActual;
    double SpindleOverride;
    double SpindleMax;
    VARIANT_BOOL SpindleAtZero;
    VARIANT_BOOL SpindleAtSpeed;
    long ErrorLevel;
    VARIANT_BOOL SingleBlockOn;
    VARIANT_BOOL OptionalStopOn;
    VARIANT_BOOL BlockSkipOn;
    VARIANT_BOOL DryRunOn;
    BSTR CurrentProgramPath;
    long CurrentProgID;
    long CurrentProgLineCount;
    long CurrentProgLine;
    long CurrentProgParseLine;
    long CurrentProgRepeat;
    long CurrentProgRepeatCount;
    long CurrentTool;
    long NextTool;
    long ToolPot;
    VARIANT_BOOL CoolantFloodOn;
    VARIANT_BOOL CoolantMistOn;
    SAFEARRAY * GCodeGroup;
    SAFEARRAY * LocalVariable;
    SAFEARRAY * CommonVariable;
    SAFEARRAY * StaticVariable;
    } 	NCDATA;


EXTERN_C const IID LIBID_PCOMMSERVERLib;

#ifndef __IPmacDevice_INTERFACE_DEFINED__
#define __IPmacDevice_INTERFACE_DEFINED__

/* interface IPmacDevice */
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_IPmacDevice;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8A00B576-9C14-425b-8878-493C91D8DE5C")
    IPmacDevice : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Open( 
            /* [in] */ long dwDevice,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Close( 
            /* [in] */ long dwDevice) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DataStart( 
            /* [in] */ long dwDevice,
            /* [in] */ long resolution,
            /* [in] */ long period) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DataStop( 
            /* [in] */ long dwDevice) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DataCollect( 
            /* [in] */ long dwDevice,
            /* [out][in] */ VARIANT *pvArray,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DPRBackGroundVar( 
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT_BOOL bOn) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DPRAddressRange( 
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT_BOOL bUpper,
            /* [in] */ VARIANT_BOOL bPC,
            /* [out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetPmacType( 
            /* [in] */ long dwDevice,
            /* [out] */ DEVPMACTYPE *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DPRVarBufRemove( 
            /* [in] */ long dwDevice,
            /* [in] */ long index,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DPRVarBufRead( 
            /* [in] */ long dwDevice,
            /* [in] */ long h,
            /* [in] */ long entryNum,
            /* [out][in] */ VARIANT *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DPRGetFloat( 
            /* [in] */ long dwDevice,
            /* [in] */ long offset,
            /* [out] */ float *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DPRGetLong( 
            /* [in] */ long dwDevice,
            /* [in] */ long offset,
            /* [out] */ long *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DPRVarBufInit( 
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT vAddressArray,
            /* [in] */ VARIANT vTypeArray,
            /* [out] */ long *pHandle,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DPRSetFloat( 
            /* [in] */ long dwDevice,
            /* [in] */ long offset,
            /* [in] */ float newVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DPRSetLong( 
            /* [in] */ long dwDevice,
            /* [in] */ long offset,
            /* [in] */ long newVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DPRVarBufWrite( 
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT vData,
            /* [in] */ VARIANT vAddresses,
            /* [in] */ VARIANT vTypes,
            /* [out] */ long *pWritten) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DPRFloat( 
            /* [in] */ __int64 d,
            /* [in] */ double scale,
            /* [out] */ double *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetRomVersion( 
            /* [in] */ long dwDevice,
            /* [out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetRomDate( 
            /* [in] */ long dwDevice,
            /* [out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetPmacLocation( 
            /* [in] */ long dwDevice,
            /* [out] */ DEVLOCATIONTYPE *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetResponse( 
            /* [in] */ long dwDevice,
            /* [in] */ BSTR question,
            /* [out] */ BSTR *pAnswer) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SelectDevice( 
            /* [in] */ long hWindow,
            /* [out] */ long *pDeviceNumber,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DPRSize( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DPRAvailable( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetChecksums( 
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT_BOOL bActive) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetControlResponse( 
            /* [in] */ long dwDevice,
            /* [in] */ short question,
            /* [out] */ BSTR *pAnswer) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_LastCommError( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetLastCommErrorStr( 
            /* [in] */ long dwDevice,
            /* [out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetMacroIVariableLong( 
            /* [in] */ long dwDevice,
            /* [in] */ long node,
            /* [in] */ long num,
            /* [in] */ long def,
            /* [out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DoChecksums( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_DoChecksums( 
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetIVariableLong( 
            /* [in] */ long dwDevice,
            /* [in] */ long num,
            /* [in] */ long def,
            /* [out] */ long *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetIVariableDouble( 
            /* [in] */ long dwDevice,
            /* [in] */ long num,
            /* [in] */ double def,
            /* [out] */ double *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE SetIVariableLong( 
            /* [in] */ long dwDevice,
            /* [in] */ long num,
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE SetIVariableDouble( 
            /* [in] */ long dwDevice,
            /* [in] */ long num,
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAsciiComm( 
            /* [in] */ long dwDevice,
            /* [out] */ DEVASCIIMODE *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetAsciiComm( 
            /* [in] */ long dwDevice,
            /* [in] */ DEVASCIIMODE newVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetVariableLong( 
            /* [in] */ long dwDevice,
            /* [in] */ short ch,
            /* [in] */ long num,
            /* [in] */ long def,
            /* [out] */ long *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetVariableDouble( 
            /* [in] */ long dwDevice,
            /* [in] */ short ch,
            /* [in] */ long num,
            /* [in] */ double def,
            /* [out] */ double *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE SetVariableLong( 
            /* [in] */ long dwDevice,
            /* [in] */ short ch,
            /* [in] */ long num,
            /* [in] */ long newVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE SetVariableDouble( 
            /* [in] */ long dwDevice,
            /* [in] */ short ch,
            /* [in] */ long num,
            /* [in] */ double newVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_InBootStrapMode( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MotionBufferOpen( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_RotaryBufferOpen( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_IsTurbo( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE RestoreLinkList( 
            /* [in] */ BSTR firmwareVersion,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE BackupLinkList( 
            /* [in] */ long dwDevice,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetCriticalIVars( 
            /* [in] */ long dwDevice,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetLocAndEnum( 
            /* [in] */ long dwDevice,
            /* [out] */ long *pLoc,
            /* [out] */ long *pEnum,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetProgramInfo( 
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT_BOOL bPlc,
            /* [in] */ long num,
            /* [out] */ long *pSAdr,
            /* [out] */ long *pFAdr) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DownloadFile( 
            /* [in] */ long dwDevice,
            /* [in] */ BSTR filePath) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AbortDownload( 
            /* [in] */ long dwDevice) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DownloadFirmwareFile( 
            /* [in] */ long dwDevice,
            /* [in] */ BSTR filePath,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Download( 
            /* [in] */ long dwDevice,
            /* [in] */ BSTR filePath,
            /* [in] */ VARIANT_BOOL bMacro,
            /* [in] */ VARIANT_BOOL bMap,
            /* [in] */ VARIANT_BOOL bLog,
            /* [in] */ VARIANT_BOOL bDnld,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Downloading( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_MaxDownloadErrors( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propput] */ HRESULT STDMETHODCALLTYPE put_MaxDownloadErrors( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE CompilePLCC( 
            /* [in] */ long dwDevice,
            /* [in] */ BSTR inFilePath,
            /* [in] */ BSTR outFilePath,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE SendCommand( 
            /* [in] */ long dwDevice,
            /* [in] */ BSTR cmd,
            /* [out] */ long *pStatus) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetPlcStatus( 
            /* [in] */ long dwDevice,
            /* [out] */ long *pNumPlc,
            /* [out] */ long *pNumPlcc,
            /* [out] */ long *pI5) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetFirstPlcc( 
            /* [in] */ long dwDevice,
            /* [out] */ VARIANT_BOOL *pbLoaded,
            /* [out] */ VARIANT_BOOL *pbEnabled,
            /* [out] */ long *pStartAdr,
            /* [out] */ long *pLastAdr,
            /* [out] */ long *pSize,
            /* [out] */ VARIANT_BOOL *pbMore) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetNextPlcc( 
            /* [in] */ long dwDevice,
            /* [out] */ VARIANT_BOOL *pbLoaded,
            /* [out] */ VARIANT_BOOL *pbEnabled,
            /* [out] */ long *pStartAdr,
            /* [out] */ long *pLastAdr,
            /* [out] */ long *pSize,
            /* [out] */ VARIANT_BOOL *pbMore) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetFirstPlc( 
            /* [in] */ long dwDevice,
            /* [out] */ VARIANT_BOOL *pbEnabled,
            /* [out] */ long *pStartAdr,
            /* [out] */ long *pLastAdr,
            /* [out] */ long *pSize,
            /* [out] */ VARIANT_BOOL *pbMore) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetNextPlc( 
            /* [in] */ long dwDevice,
            /* [out] */ VARIANT_BOOL *pbEnabled,
            /* [out] */ long *pStartAdr,
            /* [out] */ long *pLastAdr,
            /* [out] */ long *pSize,
            /* [out] */ VARIANT_BOOL *pbMore) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IsAnyPlcRunning( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetGlobalStatus( 
            /* [in] */ long dwDevice,
            /* [out] */ long *pStatusX,
            /* [out] */ long *pStatusY,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetCoordStatus( 
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [out] */ long *pStatusX,
            /* [out] */ long *pStatusY,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetMotorStatus( 
            /* [in] */ long dwDevice,
            /* [in] */ long lMotor,
            /* [out] */ long *pStatusX,
            /* [out] */ long *pStatusY,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_SysServoError( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_SysReEntryError( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_SysMemChecksumError( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_SysPromChecksumError( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_SysInposition( 
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_SysWarnFError( 
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_SysFatalFError( 
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_SysRuntimeError( 
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_SysCircleRadError( 
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_SysAmpFaultError( 
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetCommandedPos( 
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [in] */ double scale,
            /* [out] */ double *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetPosition( 
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [in] */ double scale,
            /* [out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetNetActualPosition( 
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [in] */ double scale,
            /* [out] */ double *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetFollowError( 
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [in] */ double scale,
            /* [out] */ double *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetVelocity( 
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [in] */ double scale,
            /* [out] */ double *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetMasterPos( 
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [in] */ double scale,
            /* [out] */ double *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetCompensationPos( 
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [in] */ double scale,
            /* [out] */ double *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetTargetPos( 
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [in] */ double scale,
            /* [out] */ double *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetBiasPos( 
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [in] */ double scale,
            /* [out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetProgExecPoint( 
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [out] */ long *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetRotBufFull( 
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ProgramRunning( 
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_ProgramStepping( 
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_ProgramContMotion( 
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_ProgramContRequest( 
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_ProgramRemaining( 
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_MotionBufOpen( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_ProgramFeedMode( 
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_MotorAmpEnabled( 
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_MotorWarnFError( 
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_MotorFatalFError( 
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_MotorAmpFault( 
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_MotorOnPositionLimit( 
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_MotorHomeComplete( 
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_MotorInPosition( 
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_TimRemInMove( 
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_TimeRemInTATS( 
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_MotorDataBlockError( 
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_MotorPhased( 
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_MotorEnabled( 
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_MotorHandwheelEnabled( 
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_MotorOpenLoop( 
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_MotorOnNegLimit( 
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_MotorOnPosLimit( 
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE SetJogReturn( 
            /* [in] */ long dwDevice,
            /* [in] */ long mtr) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetWhyMotorNotMoving( 
            /* [in] */ long dwDevice,
            /* [in] */ long motor,
            /* [out] */ long *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetWhyMotorNotMovingStr( 
            /* [in] */ long err,
            /* [out][in] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetWhyCSNotMoving( 
            /* [in] */ long dwDevice,
            /* [in] */ long cs,
            /* [out] */ long *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetWhyCSNotMovingStr( 
            /* [in] */ long err,
            /* [out][in] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_BufferOpen( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_GatherPeriod( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_GatherPeriod( 
            /* [in] */ long dwDevice,
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_GatherEnabled( 
            /* [in] */ long dwDevice,
            /* [in] */ long num,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_GatherEnabled( 
            /* [in] */ long dwDevice,
            /* [in] */ long num,
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_GatherMode( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ DEVGATMODE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_GatherMode( 
            /* [in] */ long dwDevice,
            /* [in] */ DEVGATMODE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_GatherSampleTime( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_GatherSampleTime( 
            /* [in] */ long dwDevice,
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetNumGatherSources( 
            /* [in] */ long dwDevice,
            /* [out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetNumGatherSamples( 
            /* [in] */ long dwDevice,
            /* [out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetGather( 
            /* [in] */ long dwDevice,
            /* [in] */ long num,
            /* [in] */ BSTR str,
            /* [in] */ VARIANT_BOOL bEnable,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetQuickGather( 
            /* [in] */ long dwDevice,
            /* [in] */ long lComMask,
            /* [in] */ long lEncMask,
            /* [in] */ long lDacMask,
            /* [in] */ long lCurMask,
            /* [in] */ VARIANT_BOOL bEnable,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetQuickGatherWithDirectCurrent( 
            /* [in] */ long dwDevice,
            /* [in] */ long lComMask,
            /* [in] */ long lEncMask,
            /* [in] */ long lDacMask,
            /* [in] */ long lCurMask,
            /* [in] */ VARIANT_BOOL bEnable,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetGather( 
            /* [in] */ long dwDevice,
            /* [in] */ long num,
            /* [out][in] */ BSTR *pStr,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ClearGather( 
            /* [in] */ long dwDevice) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE InitGather( 
            /* [in] */ long dwDevice,
            /* [in] */ long size,
            /* [in] */ double msec,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ClearGatherData( 
            /* [in] */ long dwDevice) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CollectGatherData( 
            /* [in] */ long dwDevice,
            /* [out] */ long *pSources,
            /* [out] */ long *pSamples,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetGatherSamples( 
            /* [in] */ long dwDevice,
            /* [in] */ long sourceNum,
            /* [out][in] */ VARIANT *pVariant,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetGatherPoint( 
            /* [in] */ long dwDevice,
            /* [in] */ long sourceNum,
            /* [in] */ long sampleNum,
            /* [out] */ double *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartGather( 
            /* [in] */ long dwDevice,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StopGather( 
            /* [in] */ long dwDevice) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE InitPlotGather( 
            /* [in] */ long dwDevice,
            /* [in] */ long size,
            /* [in] */ long period,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE ReadGatherFile( 
            /* [in] */ long dwDevice,
            /* [in] */ BSTR bsFilePath,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE WriteGatherFile( 
            /* [in] */ long dwDevice,
            /* [in] */ BSTR bsFilePath,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE strtod48f( 
            /* [in] */ BSTR str,
            /* [out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE strtod48l( 
            /* [in] */ BSTR str,
            /* [out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE strtod24( 
            /* [in] */ BSTR str,
            /* [out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE strtod32dp( 
            /* [in] */ BSTR str,
            /* [out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE strto32f( 
            /* [in] */ BSTR str,
            /* [out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE UploadMacroConfig( 
            /* [in] */ long dwDevice,
            /* [in] */ BSTR bsFilePath,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DownloadCurrentFeedbackTest( 
            /* [in] */ long dwDevice,
            /* [in] */ short mtr,
            /* [in] */ double pwmDutyCycle,
            /* [in] */ short pwmFreq,
            /* [in] */ VARIANT_BOOL bMacro,
            /* [in] */ VARIANT_BOOL bUltra,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE ReadResultsFromCurrentFeedbackTest( 
            /* [in] */ long dwDevice,
            /* [in] */ short mtr,
            /* [out] */ long *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DownloadVoltageSixStepTest( 
            /* [in] */ long dwDevice,
            /* [in] */ short mtr,
            /* [in] */ double pwmDutyCycle,
            /* [in] */ VARIANT_BOOL bMacro,
            /* [in] */ VARIANT_BOOL bUltra,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE ReadResultsFromSixStepTest( 
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT_BOOL bSipFirstStep,
            /* [out][in] */ VARIANT *pvStepArray,
            /* [out] */ long *pResult) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DownloadCurrentSixStepTest( 
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ double pwmDutyCycle,
            /* [in] */ VARIANT_BOOL bMacro,
            /* [in] */ VARIANT_BOOL bUltra,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DownloadOpenLoopTest( 
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ short magnitude,
            /* [in] */ short time,
            /* [in] */ short reps,
            /* [in] */ VARIANT_BOOL bGatherCurrents,
            /* [in] */ VARIANT_BOOL bMacro,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DownloadOpenLoopTest1( 
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ short magnitude,
            /* [in] */ short time,
            /* [in] */ short reps,
            /* [in] */ VARIANT_BOOL bGatherCurrents,
            /* [in] */ VARIANT_BOOL bMacro,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DownloadVelLimitPLC( 
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ long maxspd,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DownloadPosLimitPLC( 
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DownloadUVWSixStepTest( 
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ double pwmDutyCycle,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE ReadResultsFromUVWSixStepTest( 
            /* [in] */ long dwDevice,
            /* [out][in] */ VARIANT *pvArray,
            /* [out] */ long *pResult) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE SetIVarForFeedback( 
            /* [in] */ long dwDevice,
            /* [in] */ short IVar,
            /* [in] */ long address,
            /* [in] */ DEVFEEDBACKTYPE fbType,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DownloadAndRunRoughPhasePLC( 
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE CreateIncrementalFinePhasePLC( 
            /* [in] */ long dwDevice,
            /* [in] */ BSTR filePath,
            /* [in] */ short PLCNumber,
            /* [in] */ short motor,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DoACurrentLoopStep( 
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ short dwellTime,
            /* [in] */ long magnitude,
            /* [in] */ VARIANT_BOOL bAutoTune,
            /* [out] */ BSTR *pbsCTSCurrent,
            /* [in] */ double pwmDutyCycle,
            /* [in] */ short divisor,
            /* [in] */ VARIANT_BOOL bUltra) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DoACurrentLoopStepBrush( 
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ short dwellTime,
            /* [in] */ long magnitude,
            /* [in] */ VARIANT_BOOL bAutoTune,
            /* [out] */ BSTR *pbsCTSCurrent,
            /* [in] */ double pwmDutyCycle,
            /* [in] */ short divisor,
            /* [in] */ VARIANT_BOOL bUltra) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DoAPositionLoopStep( 
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ short stepSize,
            /* [in] */ short stepTime,
            /* [in] */ VARIANT_BOOL bBiDirectional,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DoAParabolicMove( 
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ long stepSize,
            /* [in] */ short stepTime,
            /* [in] */ VARIANT_BOOL bBiDirectional,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DoAGantryPositionLoopStep( 
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ short motor2,
            /* [in] */ short stepSize,
            /* [in] */ short stepTime,
            /* [in] */ VARIANT_BOOL bBiDirectional,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DoAGantryParabolicMove( 
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ short motor2,
            /* [in] */ long stepSize,
            /* [in] */ short stepTime,
            /* [in] */ VARIANT_BOOL bBiDirectional,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DoAGantryParabolic( 
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ short motor2,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE ConvertWordToStringFloat( 
            /* [in] */ BSTR word,
            /* [out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE ConvertWordToShort( 
            /* [in] */ BSTR word,
            /* [in] */ short offset,
            /* [out] */ short *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE ConvertWordToLong( 
            /* [in] */ BSTR word,
            /* [out] */ long *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE RestoreModifiedIVars( 
            /* [in] */ long dwDevice,
            /* [in] */ short motor) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE CalculateStepStatistics( 
            /* [out][in] */ VARIANT *pvData,
            /* [in] */ long stepSize,
            /* [in] */ long I10,
            /* [in] */ long I19,
            /* [out] */ BSTR *pbsRiseTime,
            /* [out] */ BSTR *pbsPeakTime,
            /* [out] */ BSTR *pbsNatFreq,
            /* [out] */ BSTR *pbsOvershoot,
            /* [out] */ BSTR *pbsDamping,
            /* [out] */ BSTR *pbsSettleTime) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE LowPassFilterTheData( 
            /* [out][in] */ VARIANT *pvData,
            /* [in] */ BSTR bsCurrent) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DifferentiateTheData( 
            /* [out][in] */ VARIANT *pvData,
            /* [in] */ long I10,
            /* [in] */ long I19,
            /* [out] */ long *pMinVal,
            /* [out] */ long *pMaxVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DownloadAutoTuningPLC( 
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ VARIANT_BOOL bCurrentLoop,
            /* [in] */ VARIANT_BOOL bBypassJog,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DownloadGantryAutoTuningPLCP1( 
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ short motor2,
            /* [in] */ VARIANT_BOOL bCurrentLoop,
            /* [in] */ VARIANT_BOOL bBypassJog,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DownloadGantryAutoTuningPLC( 
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ short motor2,
            /* [in] */ VARIANT_BOOL bCurrentLoop,
            /* [in] */ VARIANT_BOOL bBypassJog,
            /* [in] */ double MaxTravel,
            /* [in] */ long time,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE MagEstimation( 
            /* [in] */ long dwDevice,
            /* [in] */ BSTR filePath,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE SlopeEstimation( 
            /* [in] */ long dwDevice,
            /* [in] */ BSTR filePath,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetCurrentGather( 
            /* [in] */ long dwDevice,
            /* [in] */ long mask,
            /* [in] */ VARIANT_BOOL bEnable,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DownloadOpenLoopTestForPMAC1( 
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ short magnitude,
            /* [in] */ short time,
            /* [in] */ short reps,
            /* [in] */ VARIANT_BOOL bGatherCurrents,
            /* [in] */ VARIANT_BOOL bMacro,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetMacroNodes( 
            /* [in] */ long dwDevice,
            /* [out] */ long *pNumMacroStations,
            /* [out] */ long *pNumActiveNodes) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetMacroNodeInfo( 
            /* [in] */ long dwDevice,
            /* [in] */ long modeNum,
            /* [out] */ VARIANT_BOOL *pbEnabled,
            /* [out] */ VARIANT_BOOL *pbIsIONode,
            /* [out] */ long *pID) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE SetMacroIVariableString( 
            /* [in] */ long dwDevice,
            /* [in] */ long node,
            /* [in] */ long num,
            /* [in] */ BSTR bsMIVarValue,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetMacroIVariableString( 
            /* [in] */ long dwDevice,
            /* [in] */ long node,
            /* [in] */ long num,
            /* [in] */ BSTR bsDefault,
            /* [out] */ BSTR *pbsVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetCommandLogging( 
            /* [in] */ long dwDevice,
            /* [out] */ VARIANT_BOOL *pbON) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE SetCommandLogging( 
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT_BOOL bON,
            /* [in] */ BSTR bsFilePath,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetEnumeration( 
            /* [in] */ long dwDevice,
            /* [out] */ long *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetControlValues( 
            /* [in] */ long dwDevice,
            /* [in] */ short ctrlCh,
            /* [out][in] */ VARIANT *pVariant,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE MultiDownload( 
            /* [in] */ long dwDevice,
            /* [in] */ BSTR outFile,
            /* [in] */ BSTR iniFile,
            /* [in] */ BSTR userID,
            /* [in] */ VARIANT_BOOL bMacro,
            /* [in] */ VARIANT_BOOL bMap,
            /* [in] */ VARIANT_BOOL bLog,
            /* [in] */ VARIANT_BOOL bDnld,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE AddDownloadFile( 
            /* [in] */ long dwDevice,
            /* [in] */ BSTR iniFile,
            /* [in] */ BSTR userID,
            /* [in] */ BSTR dlFile,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE RemoveDownloadFile( 
            /* [in] */ long dwDevice,
            /* [in] */ BSTR iniFile,
            /* [in] */ BSTR userId,
            /* [in] */ BSTR dlFile,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE RenumberFiles( 
            /* [in] */ long dwDevice,
            /* [in] */ long fileNumber,
            /* [in] */ BSTR iniFile) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetVariableStr( 
            /* [in] */ long dwDevice,
            /* [in] */ short ch,
            /* [in] */ long num,
            /* [out] */ BSTR *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DPRVarBufReadAll( 
            /* [in] */ long dwDevice,
            /* [in] */ long h,
            /* [out][in] */ VARIANT *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE UltraLightAnalyze( 
            /* [in] */ long dwDevice,
            /* [out][in] */ VARIANT *pVal,
            /* [out] */ long *pActiveNodes) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetPlcStatusStructure( 
            /* [in] */ long dwDevice,
            /* [out][in] */ VARIANT *pVal,
            /* [out] */ long *pPrograms) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetBitValue( 
            /* [in] */ BSTR hexStr,
            /* [in] */ short bit,
            /* [out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetNumDeviceKeys( 
            /* [out] */ long *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DPRGetLongDouble( 
            /* [in] */ LONG dwDevice,
            /* [in] */ LONG address,
            /* [out] */ DOUBLE *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DPRGetFixedDouble( 
            /* [in] */ LONG dwDevice,
            /* [in] */ LONG address,
            /* [out] */ DOUBLE *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DPRGetShort( 
            /* [in] */ LONG dwDevice,
            /* [in] */ LONG address,
            /* [out] */ SHORT *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DPRSetShort( 
            /* [in] */ LONG dwDevice,
            /* [in] */ LONG address,
            /* [in] */ SHORT newVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DPRDWordBitSet( 
            /* [in] */ LONG dwDevice,
            /* [in] */ LONG offset,
            /* [in] */ LONG bit,
            /* [out] */ VARIANT_BOOL *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DPRResetDWordBit( 
            /* [in] */ LONG dwDevice,
            /* [in] */ LONG offset,
            /* [in] */ LONG bit,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DPRSetDWordBit( 
            /* [in] */ LONG dwDevice,
            /* [in] */ LONG offset,
            /* [in] */ LONG bit,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetFirmwareType( 
            /* [in] */ LONG dwDevice,
            /* [out] */ DEVPMACFIRMWARETYPE *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetResponseEx( 
            /* [in] */ long dwDevice,
            /* [in] */ BSTR question,
            /* [in] */ VARIANT_BOOL bAddLF,
            /* [out] */ BSTR *pAnswer,
            /* [out] */ long *pStatus) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DPRVarBufWriteEx( 
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT vData,
            /* [out] */ long *pWritten) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DPRGetDWordMask( 
            /* [in] */ long dwDevice,
            /* [in] */ long offset,
            /* [in] */ long mask,
            /* [out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DPRSetDWordMask( 
            /* [in] */ long dwDevice,
            /* [in] */ long offset,
            /* [in] */ long mask,
            /* [in] */ long Val) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DPRRotBufRemove( 
            /* [in] */ long dwDevice,
            /* [in] */ long bufnum) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DPRRotBufInit( 
            /* [in] */ long dwDevice,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DPRRotBufClr( 
            /* [in] */ long dwDevice,
            /* [in] */ long bufnum) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DPRSetRotBuf( 
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT_BOOL on) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DPRAsciiStrToRotEx( 
            /* [in] */ long dwDevice,
            /* [in] */ BSTR inpstr,
            /* [in] */ long bufnum,
            /* [in] */ VARIANT_BOOL bSendRemaining,
            /* [out] */ long *pStatus) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DPRAsciiStrToRot( 
            /* [in] */ long dwDevice,
            /* [in] */ BSTR inpstr,
            /* [in] */ long bufnum,
            /* [out] */ long *pStatus) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetProgramMode( 
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [out] */ DEVPROGRAMMODE *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetProgramMotionMode( 
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [out] */ DEVMOTIONMODE *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE MapDeviceNumToEnum( 
            /* [in] */ long location,
            /* [in] */ long enumeration,
            /* [out] */ long *pDeviceNumber) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE MapLocAndEnumFromDeviceNum( 
            /* [in] */ long dwDevice,
            /* [out] */ long *pLocation,
            /* [out] */ long *pEnumeration) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetFeedRateAndMode( 
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [out] */ double *pRate,
            /* [out] */ VARIANT_BOOL *pMode) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE GetMotorMotionMode( 
            /* [in] */ long dwDevice,
            /* [in] */ long dwMotor,
            /* [out] */ DEVMOTION *pVal) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE CommandLog( 
            /* [in] */ long dwDevice,
            /* [in] */ BSTR bStr,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DPRGetMem( 
            /* [in] */ LONG dwDevice,
            /* [in] */ LONG offset,
            /* [in] */ LONG bytes,
            /* [out][in] */ VARIANT *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DPRSetMem( 
            /* [in] */ LONG dwDevice,
            /* [in] */ LONG offset,
            /* [in] */ LONG bytes,
            /* [in] */ VARIANT Val,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetResponseProgress( 
            /* [in] */ long dwDevice,
            /* [in] */ BSTR question,
            /* [in] */ VARIANT_BOOL bAddLF,
            /* [out] */ BSTR *pAnswer,
            /* [out] */ long *pStatus) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Abort( 
            /* [in] */ long dwDevice) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_InterruptsEnabled( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_InterruptsEnabled( 
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_InterruptMask( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_InterruptMask( 
            /* [in] */ long dwDevice,
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PmacSave( 
            /* [in] */ long dwDevice,
            /* [in] */ BSTR question,
            /* [out] */ long *pStatus) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PmacReset( 
            /* [in] */ long dwDevice,
            /* [in] */ BSTR question,
            /* [in] */ VARIANT_BOOL bAsciiRingComm,
            /* [out] */ long *pStatus) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PhaseMotor( 
            /* [in] */ long dwDevice,
            /* [in] */ BSTR question,
            /* [out] */ long *pStatuss) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetControlResponseEx( 
            /* [in] */ long dwDevice,
            /* [in] */ short question,
            /* [out] */ BSTR *pAnswer,
            /* [out] */ long *pStatus) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_USMonitoringEnabled( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_USMonitoringEnabled( 
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_USMonitoringPeriod( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_USMonitoringPeriod( 
            /* [in] */ long dwDevice,
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DPRTest( 
            /* [in] */ long dwDevice,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AbortTest( 
            /* [in] */ long dwDevice) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DPRGetMotorRpt( 
            /* [in] */ long dwDevice,
            /* [in] */ long motor,
            /* [out][in] */ DEVFGMOTOR *pStruct,
            /* [out] */ long *pServo,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DPRGetMotorRptTurbo( 
            /* [in] */ long dwDevice,
            /* [in] */ long motor,
            /* [out][in] */ DEVFGMOTORTURBO *pStruct,
            /* [out] */ long *pServo,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DPRGetCoordRpt( 
            /* [in] */ long dwDevice,
            /* [in] */ long coord,
            /* [out][in] */ DEVBGCOORD *pStruct,
            /* [out] */ long *pServo,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE DPRGetCoordRptTurbo( 
            /* [in] */ long dwDevice,
            /* [in] */ long coord,
            /* [out][in] */ DEVBGCOORDTURBO *pStruct,
            /* [out] */ long *pServo,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_DPRMotorRptEnabled( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propput] */ HRESULT STDMETHODCALLTYPE put_DPRMotorRptEnabled( 
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_DPRMotorRptMask( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propput] */ HRESULT STDMETHODCALLTYPE put_DPRMotorRptMask( 
            /* [in] */ long dwDevice,
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_DPRMotorRptMSec( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propput] */ HRESULT STDMETHODCALLTYPE put_DPRMotorRptMSec( 
            /* [in] */ long dwDevice,
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_DPRCoordRptEnabled( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propput] */ HRESULT STDMETHODCALLTYPE put_DPRCoordRptEnabled( 
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_DPRCoordRptMSec( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propput] */ HRESULT STDMETHODCALLTYPE put_DPRCoordRptMSec( 
            /* [in] */ long dwDevice,
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_DPRCoordRptMax( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][hidden][id][propput] */ HRESULT STDMETHODCALLTYPE put_DPRCoordRptMax( 
            /* [in] */ long dwDevice,
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE UltraLightAnalyzeStruct( 
            /* [in] */ long dwDevice,
            /* [out][in] */ DEVULTRALIGHT_INFO *pStruct,
            /* [out] */ long *pActiveNodes) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetPlcStatusStruct( 
            /* [in] */ long dwDevice,
            /* [out][in] */ DEVTOTAL_PLC_STATUS_STRUCT *pStruct,
            /* [out] */ long *pPrograms) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE get_DPRTesting( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RawGetResponseEx( 
            /* [in] */ long dwDevice,
            /* [in] */ BSTR question,
            /* [in] */ VARIANT_BOOL bAddLF,
            /* [out] */ BSTR *pAnswer,
            /* [out] */ long *pStatus) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RawGetControlResponseEx( 
            /* [in] */ long dwDevice,
            /* [in] */ short question,
            /* [out] */ BSTR *pAnswer,
            /* [out] */ long *pStatus) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE ModBusCommand( 
            /* [in] */ LONG dwDevice,
            /* [in] */ BYTE command,
            /* [out] */ VARIANT *counters,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE ModbusAvailable( 
            /* [in] */ LONG dwDevice,
            /* [in] */ BYTE command,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DPRSetupBuffers( 
            /* [in] */ ULONG dwDevice,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DPRBackground( 
            /* [in] */ long dwDevice,
            /* [in] */ long on,
            /* [in] */ long period,
            /* [in] */ long coord,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DataCollectionEnabled( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DataCollectionResolution( 
            /* [in] */ long dwDevice,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetTurboMotorStatusEx( 
            /* [in] */ long dwDevice,
            /* [in] */ long lMotor,
            /* [out][in] */ DEVMSTURBO *pStatus,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetNTMotorStatusEx( 
            /* [in] */ long dwDevice,
            /* [in] */ long lMotor,
            /* [out][in] */ DEVMSNONTURBO *pStatus,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetTurboGlobalStatusEx( 
            /* [in] */ long dwDevice,
            /* [out][in] */ DEVGSTURBO *pStatus,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetNTGlobalStatusEx( 
            /* [in] */ long dwDevice,
            /* [out][in] */ DEVGSNONTURBO *pStatus,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetTurboCoordinateStatusEx( 
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [out][in] */ DEVCSTURBO *pStatus,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetNTCoordinateStatusEx( 
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [out][in] */ DEVCSNONTURBO *pStatus,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DataGather( 
            /* [in] */ long dwDevice,
            /* [out][in] */ SAFEARRAY * *pvArray,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPmacDeviceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPmacDevice * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPmacDevice * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPmacDevice * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPmacDevice * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPmacDevice * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPmacDevice * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPmacDevice * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Open )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Close )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DataStart )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long resolution,
            /* [in] */ long period);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DataStop )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DataCollect )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out][in] */ VARIANT *pvArray,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DPRBackGroundVar )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT_BOOL bOn);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DPRAddressRange )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT_BOOL bUpper,
            /* [in] */ VARIANT_BOOL bPC,
            /* [out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetPmacType )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out] */ DEVPMACTYPE *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DPRVarBufRemove )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long index,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DPRVarBufRead )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long h,
            /* [in] */ long entryNum,
            /* [out][in] */ VARIANT *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DPRGetFloat )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long offset,
            /* [out] */ float *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DPRGetLong )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long offset,
            /* [out] */ long *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DPRVarBufInit )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT vAddressArray,
            /* [in] */ VARIANT vTypeArray,
            /* [out] */ long *pHandle,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DPRSetFloat )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long offset,
            /* [in] */ float newVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DPRSetLong )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long offset,
            /* [in] */ long newVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DPRVarBufWrite )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT vData,
            /* [in] */ VARIANT vAddresses,
            /* [in] */ VARIANT vTypes,
            /* [out] */ long *pWritten);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DPRFloat )( 
            IPmacDevice * This,
            /* [in] */ __int64 d,
            /* [in] */ double scale,
            /* [out] */ double *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetRomVersion )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out] */ BSTR *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetRomDate )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out] */ BSTR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetPmacLocation )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out] */ DEVLOCATIONTYPE *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetResponse )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ BSTR question,
            /* [out] */ BSTR *pAnswer);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SelectDevice )( 
            IPmacDevice * This,
            /* [in] */ long hWindow,
            /* [out] */ long *pDeviceNumber,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DPRSize )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DPRAvailable )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetChecksums )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT_BOOL bActive);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetControlResponse )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short question,
            /* [out] */ BSTR *pAnswer);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LastCommError )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetLastCommErrorStr )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out] */ BSTR *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetMacroIVariableLong )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long node,
            /* [in] */ long num,
            /* [in] */ long def,
            /* [out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DoChecksums )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DoChecksums )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetIVariableLong )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long num,
            /* [in] */ long def,
            /* [out] */ long *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetIVariableDouble )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long num,
            /* [in] */ double def,
            /* [out] */ double *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *SetIVariableLong )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long num,
            /* [in] */ long newVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *SetIVariableDouble )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long num,
            /* [in] */ double newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAsciiComm )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out] */ DEVASCIIMODE *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetAsciiComm )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ DEVASCIIMODE newVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetVariableLong )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short ch,
            /* [in] */ long num,
            /* [in] */ long def,
            /* [out] */ long *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetVariableDouble )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short ch,
            /* [in] */ long num,
            /* [in] */ double def,
            /* [out] */ double *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *SetVariableLong )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short ch,
            /* [in] */ long num,
            /* [in] */ long newVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *SetVariableDouble )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short ch,
            /* [in] */ long num,
            /* [in] */ double newVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_InBootStrapMode )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MotionBufferOpen )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RotaryBufferOpen )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsTurbo )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *RestoreLinkList )( 
            IPmacDevice * This,
            /* [in] */ BSTR firmwareVersion,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *BackupLinkList )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetCriticalIVars )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetLocAndEnum )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out] */ long *pLoc,
            /* [out] */ long *pEnum,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetProgramInfo )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT_BOOL bPlc,
            /* [in] */ long num,
            /* [out] */ long *pSAdr,
            /* [out] */ long *pFAdr);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DownloadFile )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ BSTR filePath);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AbortDownload )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DownloadFirmwareFile )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ BSTR filePath,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Download )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ BSTR filePath,
            /* [in] */ VARIANT_BOOL bMacro,
            /* [in] */ VARIANT_BOOL bMap,
            /* [in] */ VARIANT_BOOL bLog,
            /* [in] */ VARIANT_BOOL bDnld,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Downloading )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MaxDownloadErrors )( 
            IPmacDevice * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][hidden][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_MaxDownloadErrors )( 
            IPmacDevice * This,
            /* [in] */ long newVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *CompilePLCC )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ BSTR inFilePath,
            /* [in] */ BSTR outFilePath,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *SendCommand )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ BSTR cmd,
            /* [out] */ long *pStatus);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetPlcStatus )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out] */ long *pNumPlc,
            /* [out] */ long *pNumPlcc,
            /* [out] */ long *pI5);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetFirstPlcc )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out] */ VARIANT_BOOL *pbLoaded,
            /* [out] */ VARIANT_BOOL *pbEnabled,
            /* [out] */ long *pStartAdr,
            /* [out] */ long *pLastAdr,
            /* [out] */ long *pSize,
            /* [out] */ VARIANT_BOOL *pbMore);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetNextPlcc )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out] */ VARIANT_BOOL *pbLoaded,
            /* [out] */ VARIANT_BOOL *pbEnabled,
            /* [out] */ long *pStartAdr,
            /* [out] */ long *pLastAdr,
            /* [out] */ long *pSize,
            /* [out] */ VARIANT_BOOL *pbMore);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetFirstPlc )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out] */ VARIANT_BOOL *pbEnabled,
            /* [out] */ long *pStartAdr,
            /* [out] */ long *pLastAdr,
            /* [out] */ long *pSize,
            /* [out] */ VARIANT_BOOL *pbMore);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetNextPlc )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out] */ VARIANT_BOOL *pbEnabled,
            /* [out] */ long *pStartAdr,
            /* [out] */ long *pLastAdr,
            /* [out] */ long *pSize,
            /* [out] */ VARIANT_BOOL *pbMore);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsAnyPlcRunning )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetGlobalStatus )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out] */ long *pStatusX,
            /* [out] */ long *pStatusY,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetCoordStatus )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [out] */ long *pStatusX,
            /* [out] */ long *pStatusY,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetMotorStatus )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long lMotor,
            /* [out] */ long *pStatusX,
            /* [out] */ long *pStatusY,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SysServoError )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SysReEntryError )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SysMemChecksumError )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SysPromChecksumError )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SysInposition )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SysWarnFError )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SysFatalFError )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SysRuntimeError )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SysCircleRadError )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SysAmpFaultError )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetCommandedPos )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [in] */ double scale,
            /* [out] */ double *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetPosition )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [in] */ double scale,
            /* [out] */ double *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetNetActualPosition )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [in] */ double scale,
            /* [out] */ double *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetFollowError )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [in] */ double scale,
            /* [out] */ double *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetVelocity )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [in] */ double scale,
            /* [out] */ double *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetMasterPos )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [in] */ double scale,
            /* [out] */ double *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetCompensationPos )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [in] */ double scale,
            /* [out] */ double *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetTargetPos )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [in] */ double scale,
            /* [out] */ double *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetBiasPos )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [in] */ double scale,
            /* [out] */ double *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetProgExecPoint )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [out] */ long *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetRotBufFull )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ProgramRunning )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ProgramStepping )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ProgramContMotion )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ProgramContRequest )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ProgramRemaining )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MotionBufOpen )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ProgramFeedMode )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MotorAmpEnabled )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MotorWarnFError )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MotorFatalFError )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MotorAmpFault )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MotorOnPositionLimit )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MotorHomeComplete )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MotorInPosition )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TimRemInMove )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TimeRemInTATS )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MotorDataBlockError )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MotorPhased )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MotorEnabled )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MotorHandwheelEnabled )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MotorOpenLoop )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MotorOnNegLimit )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MotorOnPosLimit )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long mtr,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *SetJogReturn )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long mtr);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetWhyMotorNotMoving )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long motor,
            /* [out] */ long *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetWhyMotorNotMovingStr )( 
            IPmacDevice * This,
            /* [in] */ long err,
            /* [out][in] */ BSTR *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetWhyCSNotMoving )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long cs,
            /* [out] */ long *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetWhyCSNotMovingStr )( 
            IPmacDevice * This,
            /* [in] */ long err,
            /* [out][in] */ BSTR *pVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BufferOpen )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_GatherPeriod )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_GatherPeriod )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_GatherEnabled )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long num,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_GatherEnabled )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long num,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_GatherMode )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ DEVGATMODE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_GatherMode )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ DEVGATMODE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_GatherSampleTime )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_GatherSampleTime )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ double newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetNumGatherSources )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetNumGatherSamples )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetGather )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long num,
            /* [in] */ BSTR str,
            /* [in] */ VARIANT_BOOL bEnable,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetQuickGather )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long lComMask,
            /* [in] */ long lEncMask,
            /* [in] */ long lDacMask,
            /* [in] */ long lCurMask,
            /* [in] */ VARIANT_BOOL bEnable,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetQuickGatherWithDirectCurrent )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long lComMask,
            /* [in] */ long lEncMask,
            /* [in] */ long lDacMask,
            /* [in] */ long lCurMask,
            /* [in] */ VARIANT_BOOL bEnable,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetGather )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long num,
            /* [out][in] */ BSTR *pStr,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ClearGather )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *InitGather )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long size,
            /* [in] */ double msec,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ClearGatherData )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CollectGatherData )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out] */ long *pSources,
            /* [out] */ long *pSamples,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetGatherSamples )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long sourceNum,
            /* [out][in] */ VARIANT *pVariant,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetGatherPoint )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long sourceNum,
            /* [in] */ long sampleNum,
            /* [out] */ double *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StartGather )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StopGather )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *InitPlotGather )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long size,
            /* [in] */ long period,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *ReadGatherFile )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ BSTR bsFilePath,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *WriteGatherFile )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ BSTR bsFilePath,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *strtod48f )( 
            IPmacDevice * This,
            /* [in] */ BSTR str,
            /* [out] */ double *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *strtod48l )( 
            IPmacDevice * This,
            /* [in] */ BSTR str,
            /* [out] */ double *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *strtod24 )( 
            IPmacDevice * This,
            /* [in] */ BSTR str,
            /* [out] */ double *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *strtod32dp )( 
            IPmacDevice * This,
            /* [in] */ BSTR str,
            /* [out] */ double *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *strto32f )( 
            IPmacDevice * This,
            /* [in] */ BSTR str,
            /* [out] */ double *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *UploadMacroConfig )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ BSTR bsFilePath,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DownloadCurrentFeedbackTest )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short mtr,
            /* [in] */ double pwmDutyCycle,
            /* [in] */ short pwmFreq,
            /* [in] */ VARIANT_BOOL bMacro,
            /* [in] */ VARIANT_BOOL bUltra,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *ReadResultsFromCurrentFeedbackTest )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short mtr,
            /* [out] */ long *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DownloadVoltageSixStepTest )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short mtr,
            /* [in] */ double pwmDutyCycle,
            /* [in] */ VARIANT_BOOL bMacro,
            /* [in] */ VARIANT_BOOL bUltra,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *ReadResultsFromSixStepTest )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT_BOOL bSipFirstStep,
            /* [out][in] */ VARIANT *pvStepArray,
            /* [out] */ long *pResult);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DownloadCurrentSixStepTest )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ double pwmDutyCycle,
            /* [in] */ VARIANT_BOOL bMacro,
            /* [in] */ VARIANT_BOOL bUltra,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DownloadOpenLoopTest )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ short magnitude,
            /* [in] */ short time,
            /* [in] */ short reps,
            /* [in] */ VARIANT_BOOL bGatherCurrents,
            /* [in] */ VARIANT_BOOL bMacro,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DownloadOpenLoopTest1 )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ short magnitude,
            /* [in] */ short time,
            /* [in] */ short reps,
            /* [in] */ VARIANT_BOOL bGatherCurrents,
            /* [in] */ VARIANT_BOOL bMacro,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DownloadVelLimitPLC )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ long maxspd,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DownloadPosLimitPLC )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DownloadUVWSixStepTest )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ double pwmDutyCycle,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *ReadResultsFromUVWSixStepTest )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out][in] */ VARIANT *pvArray,
            /* [out] */ long *pResult);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *SetIVarForFeedback )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short IVar,
            /* [in] */ long address,
            /* [in] */ DEVFEEDBACKTYPE fbType,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DownloadAndRunRoughPhasePLC )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *CreateIncrementalFinePhasePLC )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ BSTR filePath,
            /* [in] */ short PLCNumber,
            /* [in] */ short motor,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DoACurrentLoopStep )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ short dwellTime,
            /* [in] */ long magnitude,
            /* [in] */ VARIANT_BOOL bAutoTune,
            /* [out] */ BSTR *pbsCTSCurrent,
            /* [in] */ double pwmDutyCycle,
            /* [in] */ short divisor,
            /* [in] */ VARIANT_BOOL bUltra);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DoACurrentLoopStepBrush )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ short dwellTime,
            /* [in] */ long magnitude,
            /* [in] */ VARIANT_BOOL bAutoTune,
            /* [out] */ BSTR *pbsCTSCurrent,
            /* [in] */ double pwmDutyCycle,
            /* [in] */ short divisor,
            /* [in] */ VARIANT_BOOL bUltra);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DoAPositionLoopStep )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ short stepSize,
            /* [in] */ short stepTime,
            /* [in] */ VARIANT_BOOL bBiDirectional,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DoAParabolicMove )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ long stepSize,
            /* [in] */ short stepTime,
            /* [in] */ VARIANT_BOOL bBiDirectional,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DoAGantryPositionLoopStep )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ short motor2,
            /* [in] */ short stepSize,
            /* [in] */ short stepTime,
            /* [in] */ VARIANT_BOOL bBiDirectional,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DoAGantryParabolicMove )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ short motor2,
            /* [in] */ long stepSize,
            /* [in] */ short stepTime,
            /* [in] */ VARIANT_BOOL bBiDirectional,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DoAGantryParabolic )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ short motor2,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *ConvertWordToStringFloat )( 
            IPmacDevice * This,
            /* [in] */ BSTR word,
            /* [out] */ BSTR *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *ConvertWordToShort )( 
            IPmacDevice * This,
            /* [in] */ BSTR word,
            /* [in] */ short offset,
            /* [out] */ short *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *ConvertWordToLong )( 
            IPmacDevice * This,
            /* [in] */ BSTR word,
            /* [out] */ long *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *RestoreModifiedIVars )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short motor);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *CalculateStepStatistics )( 
            IPmacDevice * This,
            /* [out][in] */ VARIANT *pvData,
            /* [in] */ long stepSize,
            /* [in] */ long I10,
            /* [in] */ long I19,
            /* [out] */ BSTR *pbsRiseTime,
            /* [out] */ BSTR *pbsPeakTime,
            /* [out] */ BSTR *pbsNatFreq,
            /* [out] */ BSTR *pbsOvershoot,
            /* [out] */ BSTR *pbsDamping,
            /* [out] */ BSTR *pbsSettleTime);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *LowPassFilterTheData )( 
            IPmacDevice * This,
            /* [out][in] */ VARIANT *pvData,
            /* [in] */ BSTR bsCurrent);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DifferentiateTheData )( 
            IPmacDevice * This,
            /* [out][in] */ VARIANT *pvData,
            /* [in] */ long I10,
            /* [in] */ long I19,
            /* [out] */ long *pMinVal,
            /* [out] */ long *pMaxVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DownloadAutoTuningPLC )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ VARIANT_BOOL bCurrentLoop,
            /* [in] */ VARIANT_BOOL bBypassJog,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DownloadGantryAutoTuningPLCP1 )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ short motor2,
            /* [in] */ VARIANT_BOOL bCurrentLoop,
            /* [in] */ VARIANT_BOOL bBypassJog,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DownloadGantryAutoTuningPLC )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ short motor2,
            /* [in] */ VARIANT_BOOL bCurrentLoop,
            /* [in] */ VARIANT_BOOL bBypassJog,
            /* [in] */ double MaxTravel,
            /* [in] */ long time,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *MagEstimation )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ BSTR filePath,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *SlopeEstimation )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ BSTR filePath,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentGather )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long mask,
            /* [in] */ VARIANT_BOOL bEnable,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DownloadOpenLoopTestForPMAC1 )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short motor,
            /* [in] */ short magnitude,
            /* [in] */ short time,
            /* [in] */ short reps,
            /* [in] */ VARIANT_BOOL bGatherCurrents,
            /* [in] */ VARIANT_BOOL bMacro,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetMacroNodes )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out] */ long *pNumMacroStations,
            /* [out] */ long *pNumActiveNodes);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetMacroNodeInfo )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long modeNum,
            /* [out] */ VARIANT_BOOL *pbEnabled,
            /* [out] */ VARIANT_BOOL *pbIsIONode,
            /* [out] */ long *pID);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *SetMacroIVariableString )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long node,
            /* [in] */ long num,
            /* [in] */ BSTR bsMIVarValue,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetMacroIVariableString )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long node,
            /* [in] */ long num,
            /* [in] */ BSTR bsDefault,
            /* [out] */ BSTR *pbsVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetCommandLogging )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out] */ VARIANT_BOOL *pbON);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *SetCommandLogging )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT_BOOL bON,
            /* [in] */ BSTR bsFilePath,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetEnumeration )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out] */ long *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetControlValues )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short ctrlCh,
            /* [out][in] */ VARIANT *pVariant,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *MultiDownload )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ BSTR outFile,
            /* [in] */ BSTR iniFile,
            /* [in] */ BSTR userID,
            /* [in] */ VARIANT_BOOL bMacro,
            /* [in] */ VARIANT_BOOL bMap,
            /* [in] */ VARIANT_BOOL bLog,
            /* [in] */ VARIANT_BOOL bDnld,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *AddDownloadFile )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ BSTR iniFile,
            /* [in] */ BSTR userID,
            /* [in] */ BSTR dlFile,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *RemoveDownloadFile )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ BSTR iniFile,
            /* [in] */ BSTR userId,
            /* [in] */ BSTR dlFile,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *RenumberFiles )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long fileNumber,
            /* [in] */ BSTR iniFile);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetVariableStr )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short ch,
            /* [in] */ long num,
            /* [out] */ BSTR *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DPRVarBufReadAll )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long h,
            /* [out][in] */ VARIANT *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *UltraLightAnalyze )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out][in] */ VARIANT *pVal,
            /* [out] */ long *pActiveNodes);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetPlcStatusStructure )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out][in] */ VARIANT *pVal,
            /* [out] */ long *pPrograms);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetBitValue )( 
            IPmacDevice * This,
            /* [in] */ BSTR hexStr,
            /* [in] */ short bit,
            /* [out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetNumDeviceKeys )( 
            IPmacDevice * This,
            /* [out] */ long *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DPRGetLongDouble )( 
            IPmacDevice * This,
            /* [in] */ LONG dwDevice,
            /* [in] */ LONG address,
            /* [out] */ DOUBLE *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DPRGetFixedDouble )( 
            IPmacDevice * This,
            /* [in] */ LONG dwDevice,
            /* [in] */ LONG address,
            /* [out] */ DOUBLE *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DPRGetShort )( 
            IPmacDevice * This,
            /* [in] */ LONG dwDevice,
            /* [in] */ LONG address,
            /* [out] */ SHORT *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DPRSetShort )( 
            IPmacDevice * This,
            /* [in] */ LONG dwDevice,
            /* [in] */ LONG address,
            /* [in] */ SHORT newVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DPRDWordBitSet )( 
            IPmacDevice * This,
            /* [in] */ LONG dwDevice,
            /* [in] */ LONG offset,
            /* [in] */ LONG bit,
            /* [out] */ VARIANT_BOOL *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DPRResetDWordBit )( 
            IPmacDevice * This,
            /* [in] */ LONG dwDevice,
            /* [in] */ LONG offset,
            /* [in] */ LONG bit,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DPRSetDWordBit )( 
            IPmacDevice * This,
            /* [in] */ LONG dwDevice,
            /* [in] */ LONG offset,
            /* [in] */ LONG bit,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetFirmwareType )( 
            IPmacDevice * This,
            /* [in] */ LONG dwDevice,
            /* [out] */ DEVPMACFIRMWARETYPE *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetResponseEx )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ BSTR question,
            /* [in] */ VARIANT_BOOL bAddLF,
            /* [out] */ BSTR *pAnswer,
            /* [out] */ long *pStatus);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DPRVarBufWriteEx )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT vData,
            /* [out] */ long *pWritten);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DPRGetDWordMask )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long offset,
            /* [in] */ long mask,
            /* [out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DPRSetDWordMask )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long offset,
            /* [in] */ long mask,
            /* [in] */ long Val);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DPRRotBufRemove )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long bufnum);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DPRRotBufInit )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DPRRotBufClr )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long bufnum);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DPRSetRotBuf )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT_BOOL on);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DPRAsciiStrToRotEx )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ BSTR inpstr,
            /* [in] */ long bufnum,
            /* [in] */ VARIANT_BOOL bSendRemaining,
            /* [out] */ long *pStatus);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DPRAsciiStrToRot )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ BSTR inpstr,
            /* [in] */ long bufnum,
            /* [out] */ long *pStatus);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetProgramMode )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [out] */ DEVPROGRAMMODE *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetProgramMotionMode )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [out] */ DEVMOTIONMODE *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *MapDeviceNumToEnum )( 
            IPmacDevice * This,
            /* [in] */ long location,
            /* [in] */ long enumeration,
            /* [out] */ long *pDeviceNumber);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *MapLocAndEnumFromDeviceNum )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out] */ long *pLocation,
            /* [out] */ long *pEnumeration);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetFeedRateAndMode )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [out] */ double *pRate,
            /* [out] */ VARIANT_BOOL *pMode);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetMotorMotionMode )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long dwMotor,
            /* [out] */ DEVMOTION *pVal);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *CommandLog )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ BSTR bStr,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DPRGetMem )( 
            IPmacDevice * This,
            /* [in] */ LONG dwDevice,
            /* [in] */ LONG offset,
            /* [in] */ LONG bytes,
            /* [out][in] */ VARIANT *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DPRSetMem )( 
            IPmacDevice * This,
            /* [in] */ LONG dwDevice,
            /* [in] */ LONG offset,
            /* [in] */ LONG bytes,
            /* [in] */ VARIANT Val,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetResponseProgress )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ BSTR question,
            /* [in] */ VARIANT_BOOL bAddLF,
            /* [out] */ BSTR *pAnswer,
            /* [out] */ long *pStatus);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Abort )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_InterruptsEnabled )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_InterruptsEnabled )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_InterruptMask )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_InterruptMask )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PmacSave )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ BSTR question,
            /* [out] */ long *pStatus);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PmacReset )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ BSTR question,
            /* [in] */ VARIANT_BOOL bAsciiRingComm,
            /* [out] */ long *pStatus);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PhaseMotor )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ BSTR question,
            /* [out] */ long *pStatuss);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetControlResponseEx )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short question,
            /* [out] */ BSTR *pAnswer,
            /* [out] */ long *pStatus);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_USMonitoringEnabled )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_USMonitoringEnabled )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_USMonitoringPeriod )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_USMonitoringPeriod )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DPRTest )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AbortTest )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DPRGetMotorRpt )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long motor,
            /* [out][in] */ DEVFGMOTOR *pStruct,
            /* [out] */ long *pServo,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DPRGetMotorRptTurbo )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long motor,
            /* [out][in] */ DEVFGMOTORTURBO *pStruct,
            /* [out] */ long *pServo,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DPRGetCoordRpt )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long coord,
            /* [out][in] */ DEVBGCOORD *pStruct,
            /* [out] */ long *pServo,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *DPRGetCoordRptTurbo )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long coord,
            /* [out][in] */ DEVBGCOORDTURBO *pStruct,
            /* [out] */ long *pServo,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DPRMotorRptEnabled )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DPRMotorRptEnabled )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DPRMotorRptMask )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][hidden][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DPRMotorRptMask )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long newVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DPRMotorRptMSec )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][hidden][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DPRMotorRptMSec )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long newVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DPRCoordRptEnabled )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][hidden][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DPRCoordRptEnabled )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DPRCoordRptMSec )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][hidden][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DPRCoordRptMSec )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long newVal);
        
        /* [helpstring][hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DPRCoordRptMax )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][hidden][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DPRCoordRptMax )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *UltraLightAnalyzeStruct )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out][in] */ DEVULTRALIGHT_INFO *pStruct,
            /* [out] */ long *pActiveNodes);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetPlcStatusStruct )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out][in] */ DEVTOTAL_PLC_STATUS_STRUCT *pStruct,
            /* [out] */ long *pPrograms);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *get_DPRTesting )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RawGetResponseEx )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ BSTR question,
            /* [in] */ VARIANT_BOOL bAddLF,
            /* [out] */ BSTR *pAnswer,
            /* [out] */ long *pStatus);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RawGetControlResponseEx )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ short question,
            /* [out] */ BSTR *pAnswer,
            /* [out] */ long *pStatus);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *ModBusCommand )( 
            IPmacDevice * This,
            /* [in] */ LONG dwDevice,
            /* [in] */ BYTE command,
            /* [out] */ VARIANT *counters,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *ModbusAvailable )( 
            IPmacDevice * This,
            /* [in] */ LONG dwDevice,
            /* [in] */ BYTE command,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DPRSetupBuffers )( 
            IPmacDevice * This,
            /* [in] */ ULONG dwDevice,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DPRBackground )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long on,
            /* [in] */ long period,
            /* [in] */ long coord,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DataCollectionEnabled )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DataCollectionResolution )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetTurboMotorStatusEx )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long lMotor,
            /* [out][in] */ DEVMSTURBO *pStatus,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetNTMotorStatusEx )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long lMotor,
            /* [out][in] */ DEVMSNONTURBO *pStatus,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetTurboGlobalStatusEx )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out][in] */ DEVGSTURBO *pStatus,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetNTGlobalStatusEx )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out][in] */ DEVGSNONTURBO *pStatus,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetTurboCoordinateStatusEx )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [out][in] */ DEVCSTURBO *pStatus,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetNTCoordinateStatusEx )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [in] */ long dwCoord,
            /* [out][in] */ DEVCSNONTURBO *pStatus,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DataGather )( 
            IPmacDevice * This,
            /* [in] */ long dwDevice,
            /* [out][in] */ SAFEARRAY * *pvArray,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        END_INTERFACE
    } IPmacDeviceVtbl;

    interface IPmacDevice
    {
        CONST_VTBL struct IPmacDeviceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPmacDevice_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPmacDevice_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPmacDevice_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPmacDevice_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPmacDevice_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPmacDevice_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPmacDevice_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPmacDevice_Open(This,dwDevice,pbSuccess)	\
    (This)->lpVtbl -> Open(This,dwDevice,pbSuccess)

#define IPmacDevice_Close(This,dwDevice)	\
    (This)->lpVtbl -> Close(This,dwDevice)

#define IPmacDevice_DataStart(This,dwDevice,resolution,period)	\
    (This)->lpVtbl -> DataStart(This,dwDevice,resolution,period)

#define IPmacDevice_DataStop(This,dwDevice)	\
    (This)->lpVtbl -> DataStop(This,dwDevice)

#define IPmacDevice_DataCollect(This,dwDevice,pvArray,pbSuccess)	\
    (This)->lpVtbl -> DataCollect(This,dwDevice,pvArray,pbSuccess)

#define IPmacDevice_DPRBackGroundVar(This,dwDevice,bOn)	\
    (This)->lpVtbl -> DPRBackGroundVar(This,dwDevice,bOn)

#define IPmacDevice_DPRAddressRange(This,dwDevice,bUpper,bPC,pVal)	\
    (This)->lpVtbl -> DPRAddressRange(This,dwDevice,bUpper,bPC,pVal)

#define IPmacDevice_GetPmacType(This,dwDevice,pVal)	\
    (This)->lpVtbl -> GetPmacType(This,dwDevice,pVal)

#define IPmacDevice_DPRVarBufRemove(This,dwDevice,index,pbSuccess)	\
    (This)->lpVtbl -> DPRVarBufRemove(This,dwDevice,index,pbSuccess)

#define IPmacDevice_DPRVarBufRead(This,dwDevice,h,entryNum,pVal,pbSuccess)	\
    (This)->lpVtbl -> DPRVarBufRead(This,dwDevice,h,entryNum,pVal,pbSuccess)

#define IPmacDevice_DPRGetFloat(This,dwDevice,offset,pVal,pbSuccess)	\
    (This)->lpVtbl -> DPRGetFloat(This,dwDevice,offset,pVal,pbSuccess)

#define IPmacDevice_DPRGetLong(This,dwDevice,offset,pVal,pbSuccess)	\
    (This)->lpVtbl -> DPRGetLong(This,dwDevice,offset,pVal,pbSuccess)

#define IPmacDevice_DPRVarBufInit(This,dwDevice,vAddressArray,vTypeArray,pHandle,pbSuccess)	\
    (This)->lpVtbl -> DPRVarBufInit(This,dwDevice,vAddressArray,vTypeArray,pHandle,pbSuccess)

#define IPmacDevice_DPRSetFloat(This,dwDevice,offset,newVal,pbSuccess)	\
    (This)->lpVtbl -> DPRSetFloat(This,dwDevice,offset,newVal,pbSuccess)

#define IPmacDevice_DPRSetLong(This,dwDevice,offset,newVal,pbSuccess)	\
    (This)->lpVtbl -> DPRSetLong(This,dwDevice,offset,newVal,pbSuccess)

#define IPmacDevice_DPRVarBufWrite(This,dwDevice,vData,vAddresses,vTypes,pWritten)	\
    (This)->lpVtbl -> DPRVarBufWrite(This,dwDevice,vData,vAddresses,vTypes,pWritten)

#define IPmacDevice_DPRFloat(This,d,scale,pVal)	\
    (This)->lpVtbl -> DPRFloat(This,d,scale,pVal)

#define IPmacDevice_GetRomVersion(This,dwDevice,pVal)	\
    (This)->lpVtbl -> GetRomVersion(This,dwDevice,pVal)

#define IPmacDevice_GetRomDate(This,dwDevice,pVal)	\
    (This)->lpVtbl -> GetRomDate(This,dwDevice,pVal)

#define IPmacDevice_GetPmacLocation(This,dwDevice,pVal)	\
    (This)->lpVtbl -> GetPmacLocation(This,dwDevice,pVal)

#define IPmacDevice_GetResponse(This,dwDevice,question,pAnswer)	\
    (This)->lpVtbl -> GetResponse(This,dwDevice,question,pAnswer)

#define IPmacDevice_SelectDevice(This,hWindow,pDeviceNumber,pbSuccess)	\
    (This)->lpVtbl -> SelectDevice(This,hWindow,pDeviceNumber,pbSuccess)

#define IPmacDevice_get_DPRSize(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_DPRSize(This,dwDevice,pVal)

#define IPmacDevice_get_DPRAvailable(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_DPRAvailable(This,dwDevice,pVal)

#define IPmacDevice_SetChecksums(This,dwDevice,bActive)	\
    (This)->lpVtbl -> SetChecksums(This,dwDevice,bActive)

#define IPmacDevice_GetControlResponse(This,dwDevice,question,pAnswer)	\
    (This)->lpVtbl -> GetControlResponse(This,dwDevice,question,pAnswer)

#define IPmacDevice_get_LastCommError(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_LastCommError(This,dwDevice,pVal)

#define IPmacDevice_GetLastCommErrorStr(This,dwDevice,pVal)	\
    (This)->lpVtbl -> GetLastCommErrorStr(This,dwDevice,pVal)

#define IPmacDevice_GetMacroIVariableLong(This,dwDevice,node,num,def,pVal)	\
    (This)->lpVtbl -> GetMacroIVariableLong(This,dwDevice,node,num,def,pVal)

#define IPmacDevice_get_DoChecksums(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_DoChecksums(This,dwDevice,pVal)

#define IPmacDevice_put_DoChecksums(This,dwDevice,newVal)	\
    (This)->lpVtbl -> put_DoChecksums(This,dwDevice,newVal)

#define IPmacDevice_GetIVariableLong(This,dwDevice,num,def,pVal)	\
    (This)->lpVtbl -> GetIVariableLong(This,dwDevice,num,def,pVal)

#define IPmacDevice_GetIVariableDouble(This,dwDevice,num,def,pVal)	\
    (This)->lpVtbl -> GetIVariableDouble(This,dwDevice,num,def,pVal)

#define IPmacDevice_SetIVariableLong(This,dwDevice,num,newVal)	\
    (This)->lpVtbl -> SetIVariableLong(This,dwDevice,num,newVal)

#define IPmacDevice_SetIVariableDouble(This,dwDevice,num,newVal)	\
    (This)->lpVtbl -> SetIVariableDouble(This,dwDevice,num,newVal)

#define IPmacDevice_GetAsciiComm(This,dwDevice,pVal)	\
    (This)->lpVtbl -> GetAsciiComm(This,dwDevice,pVal)

#define IPmacDevice_SetAsciiComm(This,dwDevice,newVal)	\
    (This)->lpVtbl -> SetAsciiComm(This,dwDevice,newVal)

#define IPmacDevice_GetVariableLong(This,dwDevice,ch,num,def,pVal,pbSuccess)	\
    (This)->lpVtbl -> GetVariableLong(This,dwDevice,ch,num,def,pVal,pbSuccess)

#define IPmacDevice_GetVariableDouble(This,dwDevice,ch,num,def,pVal,pbSuccess)	\
    (This)->lpVtbl -> GetVariableDouble(This,dwDevice,ch,num,def,pVal,pbSuccess)

#define IPmacDevice_SetVariableLong(This,dwDevice,ch,num,newVal,pbSuccess)	\
    (This)->lpVtbl -> SetVariableLong(This,dwDevice,ch,num,newVal,pbSuccess)

#define IPmacDevice_SetVariableDouble(This,dwDevice,ch,num,newVal,pbSuccess)	\
    (This)->lpVtbl -> SetVariableDouble(This,dwDevice,ch,num,newVal,pbSuccess)

#define IPmacDevice_get_InBootStrapMode(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_InBootStrapMode(This,dwDevice,pVal)

#define IPmacDevice_get_MotionBufferOpen(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_MotionBufferOpen(This,dwDevice,pVal)

#define IPmacDevice_get_RotaryBufferOpen(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_RotaryBufferOpen(This,dwDevice,pVal)

#define IPmacDevice_get_IsTurbo(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_IsTurbo(This,dwDevice,pVal)

#define IPmacDevice_RestoreLinkList(This,firmwareVersion,pbSuccess)	\
    (This)->lpVtbl -> RestoreLinkList(This,firmwareVersion,pbSuccess)

#define IPmacDevice_BackupLinkList(This,dwDevice,pbSuccess)	\
    (This)->lpVtbl -> BackupLinkList(This,dwDevice,pbSuccess)

#define IPmacDevice_SetCriticalIVars(This,dwDevice,pbSuccess)	\
    (This)->lpVtbl -> SetCriticalIVars(This,dwDevice,pbSuccess)

#define IPmacDevice_GetLocAndEnum(This,dwDevice,pLoc,pEnum,pbSuccess)	\
    (This)->lpVtbl -> GetLocAndEnum(This,dwDevice,pLoc,pEnum,pbSuccess)

#define IPmacDevice_GetProgramInfo(This,dwDevice,bPlc,num,pSAdr,pFAdr)	\
    (This)->lpVtbl -> GetProgramInfo(This,dwDevice,bPlc,num,pSAdr,pFAdr)

#define IPmacDevice_DownloadFile(This,dwDevice,filePath)	\
    (This)->lpVtbl -> DownloadFile(This,dwDevice,filePath)

#define IPmacDevice_AbortDownload(This,dwDevice)	\
    (This)->lpVtbl -> AbortDownload(This,dwDevice)

#define IPmacDevice_DownloadFirmwareFile(This,dwDevice,filePath,pbSuccess)	\
    (This)->lpVtbl -> DownloadFirmwareFile(This,dwDevice,filePath,pbSuccess)

#define IPmacDevice_Download(This,dwDevice,filePath,bMacro,bMap,bLog,bDnld,pbSuccess)	\
    (This)->lpVtbl -> Download(This,dwDevice,filePath,bMacro,bMap,bLog,bDnld,pbSuccess)

#define IPmacDevice_get_Downloading(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_Downloading(This,dwDevice,pVal)

#define IPmacDevice_get_MaxDownloadErrors(This,pVal)	\
    (This)->lpVtbl -> get_MaxDownloadErrors(This,pVal)

#define IPmacDevice_put_MaxDownloadErrors(This,newVal)	\
    (This)->lpVtbl -> put_MaxDownloadErrors(This,newVal)

#define IPmacDevice_CompilePLCC(This,dwDevice,inFilePath,outFilePath,pbSuccess)	\
    (This)->lpVtbl -> CompilePLCC(This,dwDevice,inFilePath,outFilePath,pbSuccess)

#define IPmacDevice_SendCommand(This,dwDevice,cmd,pStatus)	\
    (This)->lpVtbl -> SendCommand(This,dwDevice,cmd,pStatus)

#define IPmacDevice_GetPlcStatus(This,dwDevice,pNumPlc,pNumPlcc,pI5)	\
    (This)->lpVtbl -> GetPlcStatus(This,dwDevice,pNumPlc,pNumPlcc,pI5)

#define IPmacDevice_GetFirstPlcc(This,dwDevice,pbLoaded,pbEnabled,pStartAdr,pLastAdr,pSize,pbMore)	\
    (This)->lpVtbl -> GetFirstPlcc(This,dwDevice,pbLoaded,pbEnabled,pStartAdr,pLastAdr,pSize,pbMore)

#define IPmacDevice_GetNextPlcc(This,dwDevice,pbLoaded,pbEnabled,pStartAdr,pLastAdr,pSize,pbMore)	\
    (This)->lpVtbl -> GetNextPlcc(This,dwDevice,pbLoaded,pbEnabled,pStartAdr,pLastAdr,pSize,pbMore)

#define IPmacDevice_GetFirstPlc(This,dwDevice,pbEnabled,pStartAdr,pLastAdr,pSize,pbMore)	\
    (This)->lpVtbl -> GetFirstPlc(This,dwDevice,pbEnabled,pStartAdr,pLastAdr,pSize,pbMore)

#define IPmacDevice_GetNextPlc(This,dwDevice,pbEnabled,pStartAdr,pLastAdr,pSize,pbMore)	\
    (This)->lpVtbl -> GetNextPlc(This,dwDevice,pbEnabled,pStartAdr,pLastAdr,pSize,pbMore)

#define IPmacDevice_get_IsAnyPlcRunning(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_IsAnyPlcRunning(This,dwDevice,pVal)

#define IPmacDevice_GetGlobalStatus(This,dwDevice,pStatusX,pStatusY,pbSuccess)	\
    (This)->lpVtbl -> GetGlobalStatus(This,dwDevice,pStatusX,pStatusY,pbSuccess)

#define IPmacDevice_GetCoordStatus(This,dwDevice,dwCoord,pStatusX,pStatusY,pbSuccess)	\
    (This)->lpVtbl -> GetCoordStatus(This,dwDevice,dwCoord,pStatusX,pStatusY,pbSuccess)

#define IPmacDevice_GetMotorStatus(This,dwDevice,lMotor,pStatusX,pStatusY,pbSuccess)	\
    (This)->lpVtbl -> GetMotorStatus(This,dwDevice,lMotor,pStatusX,pStatusY,pbSuccess)

#define IPmacDevice_get_SysServoError(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_SysServoError(This,dwDevice,pVal)

#define IPmacDevice_get_SysReEntryError(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_SysReEntryError(This,dwDevice,pVal)

#define IPmacDevice_get_SysMemChecksumError(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_SysMemChecksumError(This,dwDevice,pVal)

#define IPmacDevice_get_SysPromChecksumError(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_SysPromChecksumError(This,dwDevice,pVal)

#define IPmacDevice_get_SysInposition(This,dwDevice,dwCoord,pVal)	\
    (This)->lpVtbl -> get_SysInposition(This,dwDevice,dwCoord,pVal)

#define IPmacDevice_get_SysWarnFError(This,dwDevice,dwCoord,pVal)	\
    (This)->lpVtbl -> get_SysWarnFError(This,dwDevice,dwCoord,pVal)

#define IPmacDevice_get_SysFatalFError(This,dwDevice,dwCoord,pVal)	\
    (This)->lpVtbl -> get_SysFatalFError(This,dwDevice,dwCoord,pVal)

#define IPmacDevice_get_SysRuntimeError(This,dwDevice,dwCoord,pVal)	\
    (This)->lpVtbl -> get_SysRuntimeError(This,dwDevice,dwCoord,pVal)

#define IPmacDevice_get_SysCircleRadError(This,dwDevice,dwCoord,pVal)	\
    (This)->lpVtbl -> get_SysCircleRadError(This,dwDevice,dwCoord,pVal)

#define IPmacDevice_get_SysAmpFaultError(This,dwDevice,dwCoord,pVal)	\
    (This)->lpVtbl -> get_SysAmpFaultError(This,dwDevice,dwCoord,pVal)

#define IPmacDevice_GetCommandedPos(This,dwDevice,mtr,scale,pVal)	\
    (This)->lpVtbl -> GetCommandedPos(This,dwDevice,mtr,scale,pVal)

#define IPmacDevice_GetPosition(This,dwDevice,mtr,scale,pVal)	\
    (This)->lpVtbl -> GetPosition(This,dwDevice,mtr,scale,pVal)

#define IPmacDevice_GetNetActualPosition(This,dwDevice,mtr,scale,pVal)	\
    (This)->lpVtbl -> GetNetActualPosition(This,dwDevice,mtr,scale,pVal)

#define IPmacDevice_GetFollowError(This,dwDevice,mtr,scale,pVal)	\
    (This)->lpVtbl -> GetFollowError(This,dwDevice,mtr,scale,pVal)

#define IPmacDevice_GetVelocity(This,dwDevice,mtr,scale,pVal)	\
    (This)->lpVtbl -> GetVelocity(This,dwDevice,mtr,scale,pVal)

#define IPmacDevice_GetMasterPos(This,dwDevice,mtr,scale,pVal)	\
    (This)->lpVtbl -> GetMasterPos(This,dwDevice,mtr,scale,pVal)

#define IPmacDevice_GetCompensationPos(This,dwDevice,mtr,scale,pVal)	\
    (This)->lpVtbl -> GetCompensationPos(This,dwDevice,mtr,scale,pVal)

#define IPmacDevice_GetTargetPos(This,dwDevice,mtr,scale,pVal)	\
    (This)->lpVtbl -> GetTargetPos(This,dwDevice,mtr,scale,pVal)

#define IPmacDevice_GetBiasPos(This,dwDevice,mtr,scale,pVal)	\
    (This)->lpVtbl -> GetBiasPos(This,dwDevice,mtr,scale,pVal)

#define IPmacDevice_GetProgExecPoint(This,dwDevice,dwCoord,pVal)	\
    (This)->lpVtbl -> GetProgExecPoint(This,dwDevice,dwCoord,pVal)

#define IPmacDevice_GetRotBufFull(This,dwDevice,dwCoord,pVal)	\
    (This)->lpVtbl -> GetRotBufFull(This,dwDevice,dwCoord,pVal)

#define IPmacDevice_get_ProgramRunning(This,dwDevice,dwCoord,pVal)	\
    (This)->lpVtbl -> get_ProgramRunning(This,dwDevice,dwCoord,pVal)

#define IPmacDevice_get_ProgramStepping(This,dwDevice,dwCoord,pVal)	\
    (This)->lpVtbl -> get_ProgramStepping(This,dwDevice,dwCoord,pVal)

#define IPmacDevice_get_ProgramContMotion(This,dwDevice,dwCoord,pVal)	\
    (This)->lpVtbl -> get_ProgramContMotion(This,dwDevice,dwCoord,pVal)

#define IPmacDevice_get_ProgramContRequest(This,dwDevice,dwCoord,pVal)	\
    (This)->lpVtbl -> get_ProgramContRequest(This,dwDevice,dwCoord,pVal)

#define IPmacDevice_get_ProgramRemaining(This,dwDevice,dwCoord,pVal)	\
    (This)->lpVtbl -> get_ProgramRemaining(This,dwDevice,dwCoord,pVal)

#define IPmacDevice_get_MotionBufOpen(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_MotionBufOpen(This,dwDevice,pVal)

#define IPmacDevice_get_ProgramFeedMode(This,dwDevice,dwCoord,pVal)	\
    (This)->lpVtbl -> get_ProgramFeedMode(This,dwDevice,dwCoord,pVal)

#define IPmacDevice_get_MotorAmpEnabled(This,dwDevice,mtr,pVal)	\
    (This)->lpVtbl -> get_MotorAmpEnabled(This,dwDevice,mtr,pVal)

#define IPmacDevice_get_MotorWarnFError(This,dwDevice,mtr,pVal)	\
    (This)->lpVtbl -> get_MotorWarnFError(This,dwDevice,mtr,pVal)

#define IPmacDevice_get_MotorFatalFError(This,dwDevice,mtr,pVal)	\
    (This)->lpVtbl -> get_MotorFatalFError(This,dwDevice,mtr,pVal)

#define IPmacDevice_get_MotorAmpFault(This,dwDevice,mtr,pVal)	\
    (This)->lpVtbl -> get_MotorAmpFault(This,dwDevice,mtr,pVal)

#define IPmacDevice_get_MotorOnPositionLimit(This,dwDevice,mtr,pVal)	\
    (This)->lpVtbl -> get_MotorOnPositionLimit(This,dwDevice,mtr,pVal)

#define IPmacDevice_get_MotorHomeComplete(This,dwDevice,mtr,pVal)	\
    (This)->lpVtbl -> get_MotorHomeComplete(This,dwDevice,mtr,pVal)

#define IPmacDevice_get_MotorInPosition(This,dwDevice,mtr,pVal)	\
    (This)->lpVtbl -> get_MotorInPosition(This,dwDevice,mtr,pVal)

#define IPmacDevice_get_TimRemInMove(This,dwDevice,dwCoord,pVal)	\
    (This)->lpVtbl -> get_TimRemInMove(This,dwDevice,dwCoord,pVal)

#define IPmacDevice_get_TimeRemInTATS(This,dwDevice,dwCoord,pVal)	\
    (This)->lpVtbl -> get_TimeRemInTATS(This,dwDevice,dwCoord,pVal)

#define IPmacDevice_get_MotorDataBlockError(This,dwDevice,mtr,pVal)	\
    (This)->lpVtbl -> get_MotorDataBlockError(This,dwDevice,mtr,pVal)

#define IPmacDevice_get_MotorPhased(This,dwDevice,mtr,pVal)	\
    (This)->lpVtbl -> get_MotorPhased(This,dwDevice,mtr,pVal)

#define IPmacDevice_get_MotorEnabled(This,dwDevice,mtr,pVal)	\
    (This)->lpVtbl -> get_MotorEnabled(This,dwDevice,mtr,pVal)

#define IPmacDevice_get_MotorHandwheelEnabled(This,dwDevice,mtr,pVal)	\
    (This)->lpVtbl -> get_MotorHandwheelEnabled(This,dwDevice,mtr,pVal)

#define IPmacDevice_get_MotorOpenLoop(This,dwDevice,mtr,pVal)	\
    (This)->lpVtbl -> get_MotorOpenLoop(This,dwDevice,mtr,pVal)

#define IPmacDevice_get_MotorOnNegLimit(This,dwDevice,mtr,pVal)	\
    (This)->lpVtbl -> get_MotorOnNegLimit(This,dwDevice,mtr,pVal)

#define IPmacDevice_get_MotorOnPosLimit(This,dwDevice,mtr,pVal)	\
    (This)->lpVtbl -> get_MotorOnPosLimit(This,dwDevice,mtr,pVal)

#define IPmacDevice_SetJogReturn(This,dwDevice,mtr)	\
    (This)->lpVtbl -> SetJogReturn(This,dwDevice,mtr)

#define IPmacDevice_GetWhyMotorNotMoving(This,dwDevice,motor,pVal)	\
    (This)->lpVtbl -> GetWhyMotorNotMoving(This,dwDevice,motor,pVal)

#define IPmacDevice_GetWhyMotorNotMovingStr(This,err,pVal)	\
    (This)->lpVtbl -> GetWhyMotorNotMovingStr(This,err,pVal)

#define IPmacDevice_GetWhyCSNotMoving(This,dwDevice,cs,pVal)	\
    (This)->lpVtbl -> GetWhyCSNotMoving(This,dwDevice,cs,pVal)

#define IPmacDevice_GetWhyCSNotMovingStr(This,err,pVal)	\
    (This)->lpVtbl -> GetWhyCSNotMovingStr(This,err,pVal)

#define IPmacDevice_get_BufferOpen(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_BufferOpen(This,dwDevice,pVal)

#define IPmacDevice_get_GatherPeriod(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_GatherPeriod(This,dwDevice,pVal)

#define IPmacDevice_put_GatherPeriod(This,dwDevice,newVal)	\
    (This)->lpVtbl -> put_GatherPeriod(This,dwDevice,newVal)

#define IPmacDevice_get_GatherEnabled(This,dwDevice,num,pVal)	\
    (This)->lpVtbl -> get_GatherEnabled(This,dwDevice,num,pVal)

#define IPmacDevice_put_GatherEnabled(This,dwDevice,num,newVal)	\
    (This)->lpVtbl -> put_GatherEnabled(This,dwDevice,num,newVal)

#define IPmacDevice_get_GatherMode(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_GatherMode(This,dwDevice,pVal)

#define IPmacDevice_put_GatherMode(This,dwDevice,newVal)	\
    (This)->lpVtbl -> put_GatherMode(This,dwDevice,newVal)

#define IPmacDevice_get_GatherSampleTime(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_GatherSampleTime(This,dwDevice,pVal)

#define IPmacDevice_put_GatherSampleTime(This,dwDevice,newVal)	\
    (This)->lpVtbl -> put_GatherSampleTime(This,dwDevice,newVal)

#define IPmacDevice_GetNumGatherSources(This,dwDevice,pVal)	\
    (This)->lpVtbl -> GetNumGatherSources(This,dwDevice,pVal)

#define IPmacDevice_GetNumGatherSamples(This,dwDevice,pVal)	\
    (This)->lpVtbl -> GetNumGatherSamples(This,dwDevice,pVal)

#define IPmacDevice_SetGather(This,dwDevice,num,str,bEnable,pbSuccess)	\
    (This)->lpVtbl -> SetGather(This,dwDevice,num,str,bEnable,pbSuccess)

#define IPmacDevice_SetQuickGather(This,dwDevice,lComMask,lEncMask,lDacMask,lCurMask,bEnable,pbSuccess)	\
    (This)->lpVtbl -> SetQuickGather(This,dwDevice,lComMask,lEncMask,lDacMask,lCurMask,bEnable,pbSuccess)

#define IPmacDevice_SetQuickGatherWithDirectCurrent(This,dwDevice,lComMask,lEncMask,lDacMask,lCurMask,bEnable,pbSuccess)	\
    (This)->lpVtbl -> SetQuickGatherWithDirectCurrent(This,dwDevice,lComMask,lEncMask,lDacMask,lCurMask,bEnable,pbSuccess)

#define IPmacDevice_GetGather(This,dwDevice,num,pStr,pbSuccess)	\
    (This)->lpVtbl -> GetGather(This,dwDevice,num,pStr,pbSuccess)

#define IPmacDevice_ClearGather(This,dwDevice)	\
    (This)->lpVtbl -> ClearGather(This,dwDevice)

#define IPmacDevice_InitGather(This,dwDevice,size,msec,pbSuccess)	\
    (This)->lpVtbl -> InitGather(This,dwDevice,size,msec,pbSuccess)

#define IPmacDevice_ClearGatherData(This,dwDevice)	\
    (This)->lpVtbl -> ClearGatherData(This,dwDevice)

#define IPmacDevice_CollectGatherData(This,dwDevice,pSources,pSamples,pbSuccess)	\
    (This)->lpVtbl -> CollectGatherData(This,dwDevice,pSources,pSamples,pbSuccess)

#define IPmacDevice_GetGatherSamples(This,dwDevice,sourceNum,pVariant,pbSuccess)	\
    (This)->lpVtbl -> GetGatherSamples(This,dwDevice,sourceNum,pVariant,pbSuccess)

#define IPmacDevice_GetGatherPoint(This,dwDevice,sourceNum,sampleNum,pVal,pbSuccess)	\
    (This)->lpVtbl -> GetGatherPoint(This,dwDevice,sourceNum,sampleNum,pVal,pbSuccess)

#define IPmacDevice_StartGather(This,dwDevice,pbSuccess)	\
    (This)->lpVtbl -> StartGather(This,dwDevice,pbSuccess)

#define IPmacDevice_StopGather(This,dwDevice)	\
    (This)->lpVtbl -> StopGather(This,dwDevice)

#define IPmacDevice_InitPlotGather(This,dwDevice,size,period,pbSuccess)	\
    (This)->lpVtbl -> InitPlotGather(This,dwDevice,size,period,pbSuccess)

#define IPmacDevice_ReadGatherFile(This,dwDevice,bsFilePath,pbSuccess)	\
    (This)->lpVtbl -> ReadGatherFile(This,dwDevice,bsFilePath,pbSuccess)

#define IPmacDevice_WriteGatherFile(This,dwDevice,bsFilePath,pbSuccess)	\
    (This)->lpVtbl -> WriteGatherFile(This,dwDevice,bsFilePath,pbSuccess)

#define IPmacDevice_strtod48f(This,str,pVal)	\
    (This)->lpVtbl -> strtod48f(This,str,pVal)

#define IPmacDevice_strtod48l(This,str,pVal)	\
    (This)->lpVtbl -> strtod48l(This,str,pVal)

#define IPmacDevice_strtod24(This,str,pVal)	\
    (This)->lpVtbl -> strtod24(This,str,pVal)

#define IPmacDevice_strtod32dp(This,str,pVal)	\
    (This)->lpVtbl -> strtod32dp(This,str,pVal)

#define IPmacDevice_strto32f(This,str,pVal)	\
    (This)->lpVtbl -> strto32f(This,str,pVal)

#define IPmacDevice_UploadMacroConfig(This,dwDevice,bsFilePath,pbSuccess)	\
    (This)->lpVtbl -> UploadMacroConfig(This,dwDevice,bsFilePath,pbSuccess)

#define IPmacDevice_DownloadCurrentFeedbackTest(This,dwDevice,mtr,pwmDutyCycle,pwmFreq,bMacro,bUltra,pbSuccess)	\
    (This)->lpVtbl -> DownloadCurrentFeedbackTest(This,dwDevice,mtr,pwmDutyCycle,pwmFreq,bMacro,bUltra,pbSuccess)

#define IPmacDevice_ReadResultsFromCurrentFeedbackTest(This,dwDevice,mtr,pVal)	\
    (This)->lpVtbl -> ReadResultsFromCurrentFeedbackTest(This,dwDevice,mtr,pVal)

#define IPmacDevice_DownloadVoltageSixStepTest(This,dwDevice,mtr,pwmDutyCycle,bMacro,bUltra,pbSuccess)	\
    (This)->lpVtbl -> DownloadVoltageSixStepTest(This,dwDevice,mtr,pwmDutyCycle,bMacro,bUltra,pbSuccess)

#define IPmacDevice_ReadResultsFromSixStepTest(This,dwDevice,bSipFirstStep,pvStepArray,pResult)	\
    (This)->lpVtbl -> ReadResultsFromSixStepTest(This,dwDevice,bSipFirstStep,pvStepArray,pResult)

#define IPmacDevice_DownloadCurrentSixStepTest(This,dwDevice,motor,pwmDutyCycle,bMacro,bUltra,pbSuccess)	\
    (This)->lpVtbl -> DownloadCurrentSixStepTest(This,dwDevice,motor,pwmDutyCycle,bMacro,bUltra,pbSuccess)

#define IPmacDevice_DownloadOpenLoopTest(This,dwDevice,motor,magnitude,time,reps,bGatherCurrents,bMacro,pbSuccess)	\
    (This)->lpVtbl -> DownloadOpenLoopTest(This,dwDevice,motor,magnitude,time,reps,bGatherCurrents,bMacro,pbSuccess)

#define IPmacDevice_DownloadOpenLoopTest1(This,dwDevice,motor,magnitude,time,reps,bGatherCurrents,bMacro,pbSuccess)	\
    (This)->lpVtbl -> DownloadOpenLoopTest1(This,dwDevice,motor,magnitude,time,reps,bGatherCurrents,bMacro,pbSuccess)

#define IPmacDevice_DownloadVelLimitPLC(This,dwDevice,motor,maxspd,pbSuccess)	\
    (This)->lpVtbl -> DownloadVelLimitPLC(This,dwDevice,motor,maxspd,pbSuccess)

#define IPmacDevice_DownloadPosLimitPLC(This,dwDevice,motor,pbSuccess)	\
    (This)->lpVtbl -> DownloadPosLimitPLC(This,dwDevice,motor,pbSuccess)

#define IPmacDevice_DownloadUVWSixStepTest(This,dwDevice,motor,pwmDutyCycle,pbSuccess)	\
    (This)->lpVtbl -> DownloadUVWSixStepTest(This,dwDevice,motor,pwmDutyCycle,pbSuccess)

#define IPmacDevice_ReadResultsFromUVWSixStepTest(This,dwDevice,pvArray,pResult)	\
    (This)->lpVtbl -> ReadResultsFromUVWSixStepTest(This,dwDevice,pvArray,pResult)

#define IPmacDevice_SetIVarForFeedback(This,dwDevice,IVar,address,fbType,pbSuccess)	\
    (This)->lpVtbl -> SetIVarForFeedback(This,dwDevice,IVar,address,fbType,pbSuccess)

#define IPmacDevice_DownloadAndRunRoughPhasePLC(This,dwDevice,motor,pbSuccess)	\
    (This)->lpVtbl -> DownloadAndRunRoughPhasePLC(This,dwDevice,motor,pbSuccess)

#define IPmacDevice_CreateIncrementalFinePhasePLC(This,dwDevice,filePath,PLCNumber,motor,pbSuccess)	\
    (This)->lpVtbl -> CreateIncrementalFinePhasePLC(This,dwDevice,filePath,PLCNumber,motor,pbSuccess)

#define IPmacDevice_DoACurrentLoopStep(This,dwDevice,motor,dwellTime,magnitude,bAutoTune,pbsCTSCurrent,pwmDutyCycle,divisor,bUltra)	\
    (This)->lpVtbl -> DoACurrentLoopStep(This,dwDevice,motor,dwellTime,magnitude,bAutoTune,pbsCTSCurrent,pwmDutyCycle,divisor,bUltra)

#define IPmacDevice_DoACurrentLoopStepBrush(This,dwDevice,motor,dwellTime,magnitude,bAutoTune,pbsCTSCurrent,pwmDutyCycle,divisor,bUltra)	\
    (This)->lpVtbl -> DoACurrentLoopStepBrush(This,dwDevice,motor,dwellTime,magnitude,bAutoTune,pbsCTSCurrent,pwmDutyCycle,divisor,bUltra)

#define IPmacDevice_DoAPositionLoopStep(This,dwDevice,motor,stepSize,stepTime,bBiDirectional,pbSuccess)	\
    (This)->lpVtbl -> DoAPositionLoopStep(This,dwDevice,motor,stepSize,stepTime,bBiDirectional,pbSuccess)

#define IPmacDevice_DoAParabolicMove(This,dwDevice,motor,stepSize,stepTime,bBiDirectional,pbSuccess)	\
    (This)->lpVtbl -> DoAParabolicMove(This,dwDevice,motor,stepSize,stepTime,bBiDirectional,pbSuccess)

#define IPmacDevice_DoAGantryPositionLoopStep(This,dwDevice,motor,motor2,stepSize,stepTime,bBiDirectional,pbSuccess)	\
    (This)->lpVtbl -> DoAGantryPositionLoopStep(This,dwDevice,motor,motor2,stepSize,stepTime,bBiDirectional,pbSuccess)

#define IPmacDevice_DoAGantryParabolicMove(This,dwDevice,motor,motor2,stepSize,stepTime,bBiDirectional,pbSuccess)	\
    (This)->lpVtbl -> DoAGantryParabolicMove(This,dwDevice,motor,motor2,stepSize,stepTime,bBiDirectional,pbSuccess)

#define IPmacDevice_DoAGantryParabolic(This,dwDevice,motor,motor2,pbSuccess)	\
    (This)->lpVtbl -> DoAGantryParabolic(This,dwDevice,motor,motor2,pbSuccess)

#define IPmacDevice_ConvertWordToStringFloat(This,word,pVal)	\
    (This)->lpVtbl -> ConvertWordToStringFloat(This,word,pVal)

#define IPmacDevice_ConvertWordToShort(This,word,offset,pVal)	\
    (This)->lpVtbl -> ConvertWordToShort(This,word,offset,pVal)

#define IPmacDevice_ConvertWordToLong(This,word,pVal)	\
    (This)->lpVtbl -> ConvertWordToLong(This,word,pVal)

#define IPmacDevice_RestoreModifiedIVars(This,dwDevice,motor)	\
    (This)->lpVtbl -> RestoreModifiedIVars(This,dwDevice,motor)

#define IPmacDevice_CalculateStepStatistics(This,pvData,stepSize,I10,I19,pbsRiseTime,pbsPeakTime,pbsNatFreq,pbsOvershoot,pbsDamping,pbsSettleTime)	\
    (This)->lpVtbl -> CalculateStepStatistics(This,pvData,stepSize,I10,I19,pbsRiseTime,pbsPeakTime,pbsNatFreq,pbsOvershoot,pbsDamping,pbsSettleTime)

#define IPmacDevice_LowPassFilterTheData(This,pvData,bsCurrent)	\
    (This)->lpVtbl -> LowPassFilterTheData(This,pvData,bsCurrent)

#define IPmacDevice_DifferentiateTheData(This,pvData,I10,I19,pMinVal,pMaxVal)	\
    (This)->lpVtbl -> DifferentiateTheData(This,pvData,I10,I19,pMinVal,pMaxVal)

#define IPmacDevice_DownloadAutoTuningPLC(This,dwDevice,motor,bCurrentLoop,bBypassJog,pbSuccess)	\
    (This)->lpVtbl -> DownloadAutoTuningPLC(This,dwDevice,motor,bCurrentLoop,bBypassJog,pbSuccess)

#define IPmacDevice_DownloadGantryAutoTuningPLCP1(This,dwDevice,motor,motor2,bCurrentLoop,bBypassJog,pbSuccess)	\
    (This)->lpVtbl -> DownloadGantryAutoTuningPLCP1(This,dwDevice,motor,motor2,bCurrentLoop,bBypassJog,pbSuccess)

#define IPmacDevice_DownloadGantryAutoTuningPLC(This,dwDevice,motor,motor2,bCurrentLoop,bBypassJog,MaxTravel,time,pbSuccess)	\
    (This)->lpVtbl -> DownloadGantryAutoTuningPLC(This,dwDevice,motor,motor2,bCurrentLoop,bBypassJog,MaxTravel,time,pbSuccess)

#define IPmacDevice_MagEstimation(This,dwDevice,filePath,pbSuccess)	\
    (This)->lpVtbl -> MagEstimation(This,dwDevice,filePath,pbSuccess)

#define IPmacDevice_SlopeEstimation(This,dwDevice,filePath,pbSuccess)	\
    (This)->lpVtbl -> SlopeEstimation(This,dwDevice,filePath,pbSuccess)

#define IPmacDevice_SetCurrentGather(This,dwDevice,mask,bEnable,pbSuccess)	\
    (This)->lpVtbl -> SetCurrentGather(This,dwDevice,mask,bEnable,pbSuccess)

#define IPmacDevice_DownloadOpenLoopTestForPMAC1(This,dwDevice,motor,magnitude,time,reps,bGatherCurrents,bMacro,pbSuccess)	\
    (This)->lpVtbl -> DownloadOpenLoopTestForPMAC1(This,dwDevice,motor,magnitude,time,reps,bGatherCurrents,bMacro,pbSuccess)

#define IPmacDevice_GetMacroNodes(This,dwDevice,pNumMacroStations,pNumActiveNodes)	\
    (This)->lpVtbl -> GetMacroNodes(This,dwDevice,pNumMacroStations,pNumActiveNodes)

#define IPmacDevice_GetMacroNodeInfo(This,dwDevice,modeNum,pbEnabled,pbIsIONode,pID)	\
    (This)->lpVtbl -> GetMacroNodeInfo(This,dwDevice,modeNum,pbEnabled,pbIsIONode,pID)

#define IPmacDevice_SetMacroIVariableString(This,dwDevice,node,num,bsMIVarValue,pbSuccess)	\
    (This)->lpVtbl -> SetMacroIVariableString(This,dwDevice,node,num,bsMIVarValue,pbSuccess)

#define IPmacDevice_GetMacroIVariableString(This,dwDevice,node,num,bsDefault,pbsVal)	\
    (This)->lpVtbl -> GetMacroIVariableString(This,dwDevice,node,num,bsDefault,pbsVal)

#define IPmacDevice_GetCommandLogging(This,dwDevice,pbON)	\
    (This)->lpVtbl -> GetCommandLogging(This,dwDevice,pbON)

#define IPmacDevice_SetCommandLogging(This,dwDevice,bON,bsFilePath,pbSuccess)	\
    (This)->lpVtbl -> SetCommandLogging(This,dwDevice,bON,bsFilePath,pbSuccess)

#define IPmacDevice_GetEnumeration(This,dwDevice,pVal)	\
    (This)->lpVtbl -> GetEnumeration(This,dwDevice,pVal)

#define IPmacDevice_GetControlValues(This,dwDevice,ctrlCh,pVariant,pbSuccess)	\
    (This)->lpVtbl -> GetControlValues(This,dwDevice,ctrlCh,pVariant,pbSuccess)

#define IPmacDevice_MultiDownload(This,dwDevice,outFile,iniFile,userID,bMacro,bMap,bLog,bDnld,pbSuccess)	\
    (This)->lpVtbl -> MultiDownload(This,dwDevice,outFile,iniFile,userID,bMacro,bMap,bLog,bDnld,pbSuccess)

#define IPmacDevice_AddDownloadFile(This,dwDevice,iniFile,userID,dlFile,pbSuccess)	\
    (This)->lpVtbl -> AddDownloadFile(This,dwDevice,iniFile,userID,dlFile,pbSuccess)

#define IPmacDevice_RemoveDownloadFile(This,dwDevice,iniFile,userId,dlFile,pbSuccess)	\
    (This)->lpVtbl -> RemoveDownloadFile(This,dwDevice,iniFile,userId,dlFile,pbSuccess)

#define IPmacDevice_RenumberFiles(This,dwDevice,fileNumber,iniFile)	\
    (This)->lpVtbl -> RenumberFiles(This,dwDevice,fileNumber,iniFile)

#define IPmacDevice_GetVariableStr(This,dwDevice,ch,num,pVal,pbSuccess)	\
    (This)->lpVtbl -> GetVariableStr(This,dwDevice,ch,num,pVal,pbSuccess)

#define IPmacDevice_DPRVarBufReadAll(This,dwDevice,h,pVal,pbSuccess)	\
    (This)->lpVtbl -> DPRVarBufReadAll(This,dwDevice,h,pVal,pbSuccess)

#define IPmacDevice_UltraLightAnalyze(This,dwDevice,pVal,pActiveNodes)	\
    (This)->lpVtbl -> UltraLightAnalyze(This,dwDevice,pVal,pActiveNodes)

#define IPmacDevice_GetPlcStatusStructure(This,dwDevice,pVal,pPrograms)	\
    (This)->lpVtbl -> GetPlcStatusStructure(This,dwDevice,pVal,pPrograms)

#define IPmacDevice_GetBitValue(This,hexStr,bit,pVal)	\
    (This)->lpVtbl -> GetBitValue(This,hexStr,bit,pVal)

#define IPmacDevice_GetNumDeviceKeys(This,pVal)	\
    (This)->lpVtbl -> GetNumDeviceKeys(This,pVal)

#define IPmacDevice_DPRGetLongDouble(This,dwDevice,address,pVal,pbSuccess)	\
    (This)->lpVtbl -> DPRGetLongDouble(This,dwDevice,address,pVal,pbSuccess)

#define IPmacDevice_DPRGetFixedDouble(This,dwDevice,address,pVal,pbSuccess)	\
    (This)->lpVtbl -> DPRGetFixedDouble(This,dwDevice,address,pVal,pbSuccess)

#define IPmacDevice_DPRGetShort(This,dwDevice,address,pVal,pbSuccess)	\
    (This)->lpVtbl -> DPRGetShort(This,dwDevice,address,pVal,pbSuccess)

#define IPmacDevice_DPRSetShort(This,dwDevice,address,newVal,pbSuccess)	\
    (This)->lpVtbl -> DPRSetShort(This,dwDevice,address,newVal,pbSuccess)

#define IPmacDevice_DPRDWordBitSet(This,dwDevice,offset,bit,pVal,pbSuccess)	\
    (This)->lpVtbl -> DPRDWordBitSet(This,dwDevice,offset,bit,pVal,pbSuccess)

#define IPmacDevice_DPRResetDWordBit(This,dwDevice,offset,bit,pbSuccess)	\
    (This)->lpVtbl -> DPRResetDWordBit(This,dwDevice,offset,bit,pbSuccess)

#define IPmacDevice_DPRSetDWordBit(This,dwDevice,offset,bit,pbSuccess)	\
    (This)->lpVtbl -> DPRSetDWordBit(This,dwDevice,offset,bit,pbSuccess)

#define IPmacDevice_GetFirmwareType(This,dwDevice,pVal)	\
    (This)->lpVtbl -> GetFirmwareType(This,dwDevice,pVal)

#define IPmacDevice_GetResponseEx(This,dwDevice,question,bAddLF,pAnswer,pStatus)	\
    (This)->lpVtbl -> GetResponseEx(This,dwDevice,question,bAddLF,pAnswer,pStatus)

#define IPmacDevice_DPRVarBufWriteEx(This,dwDevice,vData,pWritten)	\
    (This)->lpVtbl -> DPRVarBufWriteEx(This,dwDevice,vData,pWritten)

#define IPmacDevice_DPRGetDWordMask(This,dwDevice,offset,mask,pVal)	\
    (This)->lpVtbl -> DPRGetDWordMask(This,dwDevice,offset,mask,pVal)

#define IPmacDevice_DPRSetDWordMask(This,dwDevice,offset,mask,Val)	\
    (This)->lpVtbl -> DPRSetDWordMask(This,dwDevice,offset,mask,Val)

#define IPmacDevice_DPRRotBufRemove(This,dwDevice,bufnum)	\
    (This)->lpVtbl -> DPRRotBufRemove(This,dwDevice,bufnum)

#define IPmacDevice_DPRRotBufInit(This,dwDevice,pbSuccess)	\
    (This)->lpVtbl -> DPRRotBufInit(This,dwDevice,pbSuccess)

#define IPmacDevice_DPRRotBufClr(This,dwDevice,bufnum)	\
    (This)->lpVtbl -> DPRRotBufClr(This,dwDevice,bufnum)

#define IPmacDevice_DPRSetRotBuf(This,dwDevice,on)	\
    (This)->lpVtbl -> DPRSetRotBuf(This,dwDevice,on)

#define IPmacDevice_DPRAsciiStrToRotEx(This,dwDevice,inpstr,bufnum,bSendRemaining,pStatus)	\
    (This)->lpVtbl -> DPRAsciiStrToRotEx(This,dwDevice,inpstr,bufnum,bSendRemaining,pStatus)

#define IPmacDevice_DPRAsciiStrToRot(This,dwDevice,inpstr,bufnum,pStatus)	\
    (This)->lpVtbl -> DPRAsciiStrToRot(This,dwDevice,inpstr,bufnum,pStatus)

#define IPmacDevice_GetProgramMode(This,dwDevice,dwCoord,pVal)	\
    (This)->lpVtbl -> GetProgramMode(This,dwDevice,dwCoord,pVal)

#define IPmacDevice_GetProgramMotionMode(This,dwDevice,dwCoord,pVal)	\
    (This)->lpVtbl -> GetProgramMotionMode(This,dwDevice,dwCoord,pVal)

#define IPmacDevice_MapDeviceNumToEnum(This,location,enumeration,pDeviceNumber)	\
    (This)->lpVtbl -> MapDeviceNumToEnum(This,location,enumeration,pDeviceNumber)

#define IPmacDevice_MapLocAndEnumFromDeviceNum(This,dwDevice,pLocation,pEnumeration)	\
    (This)->lpVtbl -> MapLocAndEnumFromDeviceNum(This,dwDevice,pLocation,pEnumeration)

#define IPmacDevice_GetFeedRateAndMode(This,dwDevice,dwCoord,pRate,pMode)	\
    (This)->lpVtbl -> GetFeedRateAndMode(This,dwDevice,dwCoord,pRate,pMode)

#define IPmacDevice_GetMotorMotionMode(This,dwDevice,dwMotor,pVal)	\
    (This)->lpVtbl -> GetMotorMotionMode(This,dwDevice,dwMotor,pVal)

#define IPmacDevice_CommandLog(This,dwDevice,bStr,pbSuccess)	\
    (This)->lpVtbl -> CommandLog(This,dwDevice,bStr,pbSuccess)

#define IPmacDevice_DPRGetMem(This,dwDevice,offset,bytes,pVal,pbSuccess)	\
    (This)->lpVtbl -> DPRGetMem(This,dwDevice,offset,bytes,pVal,pbSuccess)

#define IPmacDevice_DPRSetMem(This,dwDevice,offset,bytes,Val,pbSuccess)	\
    (This)->lpVtbl -> DPRSetMem(This,dwDevice,offset,bytes,Val,pbSuccess)

#define IPmacDevice_GetResponseProgress(This,dwDevice,question,bAddLF,pAnswer,pStatus)	\
    (This)->lpVtbl -> GetResponseProgress(This,dwDevice,question,bAddLF,pAnswer,pStatus)

#define IPmacDevice_Abort(This,dwDevice)	\
    (This)->lpVtbl -> Abort(This,dwDevice)

#define IPmacDevice_get_InterruptsEnabled(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_InterruptsEnabled(This,dwDevice,pVal)

#define IPmacDevice_put_InterruptsEnabled(This,dwDevice,newVal)	\
    (This)->lpVtbl -> put_InterruptsEnabled(This,dwDevice,newVal)

#define IPmacDevice_get_InterruptMask(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_InterruptMask(This,dwDevice,pVal)

#define IPmacDevice_put_InterruptMask(This,dwDevice,newVal)	\
    (This)->lpVtbl -> put_InterruptMask(This,dwDevice,newVal)

#define IPmacDevice_PmacSave(This,dwDevice,question,pStatus)	\
    (This)->lpVtbl -> PmacSave(This,dwDevice,question,pStatus)

#define IPmacDevice_PmacReset(This,dwDevice,question,bAsciiRingComm,pStatus)	\
    (This)->lpVtbl -> PmacReset(This,dwDevice,question,bAsciiRingComm,pStatus)

#define IPmacDevice_PhaseMotor(This,dwDevice,question,pStatuss)	\
    (This)->lpVtbl -> PhaseMotor(This,dwDevice,question,pStatuss)

#define IPmacDevice_GetControlResponseEx(This,dwDevice,question,pAnswer,pStatus)	\
    (This)->lpVtbl -> GetControlResponseEx(This,dwDevice,question,pAnswer,pStatus)

#define IPmacDevice_get_USMonitoringEnabled(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_USMonitoringEnabled(This,dwDevice,pVal)

#define IPmacDevice_put_USMonitoringEnabled(This,dwDevice,newVal)	\
    (This)->lpVtbl -> put_USMonitoringEnabled(This,dwDevice,newVal)

#define IPmacDevice_get_USMonitoringPeriod(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_USMonitoringPeriod(This,dwDevice,pVal)

#define IPmacDevice_put_USMonitoringPeriod(This,dwDevice,newVal)	\
    (This)->lpVtbl -> put_USMonitoringPeriod(This,dwDevice,newVal)

#define IPmacDevice_DPRTest(This,dwDevice,pbSuccess)	\
    (This)->lpVtbl -> DPRTest(This,dwDevice,pbSuccess)

#define IPmacDevice_AbortTest(This,dwDevice)	\
    (This)->lpVtbl -> AbortTest(This,dwDevice)

#define IPmacDevice_DPRGetMotorRpt(This,dwDevice,motor,pStruct,pServo,pbSuccess)	\
    (This)->lpVtbl -> DPRGetMotorRpt(This,dwDevice,motor,pStruct,pServo,pbSuccess)

#define IPmacDevice_DPRGetMotorRptTurbo(This,dwDevice,motor,pStruct,pServo,pbSuccess)	\
    (This)->lpVtbl -> DPRGetMotorRptTurbo(This,dwDevice,motor,pStruct,pServo,pbSuccess)

#define IPmacDevice_DPRGetCoordRpt(This,dwDevice,coord,pStruct,pServo,pbSuccess)	\
    (This)->lpVtbl -> DPRGetCoordRpt(This,dwDevice,coord,pStruct,pServo,pbSuccess)

#define IPmacDevice_DPRGetCoordRptTurbo(This,dwDevice,coord,pStruct,pServo,pbSuccess)	\
    (This)->lpVtbl -> DPRGetCoordRptTurbo(This,dwDevice,coord,pStruct,pServo,pbSuccess)

#define IPmacDevice_get_DPRMotorRptEnabled(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_DPRMotorRptEnabled(This,dwDevice,pVal)

#define IPmacDevice_put_DPRMotorRptEnabled(This,dwDevice,newVal)	\
    (This)->lpVtbl -> put_DPRMotorRptEnabled(This,dwDevice,newVal)

#define IPmacDevice_get_DPRMotorRptMask(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_DPRMotorRptMask(This,dwDevice,pVal)

#define IPmacDevice_put_DPRMotorRptMask(This,dwDevice,newVal)	\
    (This)->lpVtbl -> put_DPRMotorRptMask(This,dwDevice,newVal)

#define IPmacDevice_get_DPRMotorRptMSec(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_DPRMotorRptMSec(This,dwDevice,pVal)

#define IPmacDevice_put_DPRMotorRptMSec(This,dwDevice,newVal)	\
    (This)->lpVtbl -> put_DPRMotorRptMSec(This,dwDevice,newVal)

#define IPmacDevice_get_DPRCoordRptEnabled(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_DPRCoordRptEnabled(This,dwDevice,pVal)

#define IPmacDevice_put_DPRCoordRptEnabled(This,dwDevice,newVal)	\
    (This)->lpVtbl -> put_DPRCoordRptEnabled(This,dwDevice,newVal)

#define IPmacDevice_get_DPRCoordRptMSec(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_DPRCoordRptMSec(This,dwDevice,pVal)

#define IPmacDevice_put_DPRCoordRptMSec(This,dwDevice,newVal)	\
    (This)->lpVtbl -> put_DPRCoordRptMSec(This,dwDevice,newVal)

#define IPmacDevice_get_DPRCoordRptMax(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_DPRCoordRptMax(This,dwDevice,pVal)

#define IPmacDevice_put_DPRCoordRptMax(This,dwDevice,newVal)	\
    (This)->lpVtbl -> put_DPRCoordRptMax(This,dwDevice,newVal)

#define IPmacDevice_UltraLightAnalyzeStruct(This,dwDevice,pStruct,pActiveNodes)	\
    (This)->lpVtbl -> UltraLightAnalyzeStruct(This,dwDevice,pStruct,pActiveNodes)

#define IPmacDevice_GetPlcStatusStruct(This,dwDevice,pStruct,pPrograms)	\
    (This)->lpVtbl -> GetPlcStatusStruct(This,dwDevice,pStruct,pPrograms)

#define IPmacDevice_get_DPRTesting(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_DPRTesting(This,dwDevice,pVal)

#define IPmacDevice_RawGetResponseEx(This,dwDevice,question,bAddLF,pAnswer,pStatus)	\
    (This)->lpVtbl -> RawGetResponseEx(This,dwDevice,question,bAddLF,pAnswer,pStatus)

#define IPmacDevice_RawGetControlResponseEx(This,dwDevice,question,pAnswer,pStatus)	\
    (This)->lpVtbl -> RawGetControlResponseEx(This,dwDevice,question,pAnswer,pStatus)

#define IPmacDevice_ModBusCommand(This,dwDevice,command,counters,pbSuccess)	\
    (This)->lpVtbl -> ModBusCommand(This,dwDevice,command,counters,pbSuccess)

#define IPmacDevice_ModbusAvailable(This,dwDevice,command,pbSuccess)	\
    (This)->lpVtbl -> ModbusAvailable(This,dwDevice,command,pbSuccess)

#define IPmacDevice_DPRSetupBuffers(This,dwDevice,pbSuccess)	\
    (This)->lpVtbl -> DPRSetupBuffers(This,dwDevice,pbSuccess)

#define IPmacDevice_DPRBackground(This,dwDevice,on,period,coord,pbSuccess)	\
    (This)->lpVtbl -> DPRBackground(This,dwDevice,on,period,coord,pbSuccess)

#define IPmacDevice_get_DataCollectionEnabled(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_DataCollectionEnabled(This,dwDevice,pVal)

#define IPmacDevice_get_DataCollectionResolution(This,dwDevice,pVal)	\
    (This)->lpVtbl -> get_DataCollectionResolution(This,dwDevice,pVal)

#define IPmacDevice_GetTurboMotorStatusEx(This,dwDevice,lMotor,pStatus,pbSuccess)	\
    (This)->lpVtbl -> GetTurboMotorStatusEx(This,dwDevice,lMotor,pStatus,pbSuccess)

#define IPmacDevice_GetNTMotorStatusEx(This,dwDevice,lMotor,pStatus,pbSuccess)	\
    (This)->lpVtbl -> GetNTMotorStatusEx(This,dwDevice,lMotor,pStatus,pbSuccess)

#define IPmacDevice_GetTurboGlobalStatusEx(This,dwDevice,pStatus,pbSuccess)	\
    (This)->lpVtbl -> GetTurboGlobalStatusEx(This,dwDevice,pStatus,pbSuccess)

#define IPmacDevice_GetNTGlobalStatusEx(This,dwDevice,pStatus,pbSuccess)	\
    (This)->lpVtbl -> GetNTGlobalStatusEx(This,dwDevice,pStatus,pbSuccess)

#define IPmacDevice_GetTurboCoordinateStatusEx(This,dwDevice,dwCoord,pStatus,pbSuccess)	\
    (This)->lpVtbl -> GetTurboCoordinateStatusEx(This,dwDevice,dwCoord,pStatus,pbSuccess)

#define IPmacDevice_GetNTCoordinateStatusEx(This,dwDevice,dwCoord,pStatus,pbSuccess)	\
    (This)->lpVtbl -> GetNTCoordinateStatusEx(This,dwDevice,dwCoord,pStatus,pbSuccess)

#define IPmacDevice_DataGather(This,dwDevice,pvArray,pbSuccess)	\
    (This)->lpVtbl -> DataGather(This,dwDevice,pvArray,pbSuccess)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_Open_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_Open_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_Close_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice);


void __RPC_STUB IPmacDevice_Close_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DataStart_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long resolution,
    /* [in] */ long period);


void __RPC_STUB IPmacDevice_DataStart_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DataStop_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice);


void __RPC_STUB IPmacDevice_DataStop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DataCollect_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out][in] */ VARIANT *pvArray,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DataCollect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRBackGroundVar_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ VARIANT_BOOL bOn);


void __RPC_STUB IPmacDevice_DPRBackGroundVar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRAddressRange_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ VARIANT_BOOL bUpper,
    /* [in] */ VARIANT_BOOL bPC,
    /* [out] */ long *pVal);


void __RPC_STUB IPmacDevice_DPRAddressRange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetPmacType_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out] */ DEVPMACTYPE *pVal);


void __RPC_STUB IPmacDevice_GetPmacType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRVarBufRemove_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long index,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DPRVarBufRemove_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRVarBufRead_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long h,
    /* [in] */ long entryNum,
    /* [out][in] */ VARIANT *pVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DPRVarBufRead_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRGetFloat_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long offset,
    /* [out] */ float *pVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DPRGetFloat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRGetLong_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long offset,
    /* [out] */ long *pVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DPRGetLong_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRVarBufInit_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ VARIANT vAddressArray,
    /* [in] */ VARIANT vTypeArray,
    /* [out] */ long *pHandle,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DPRVarBufInit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRSetFloat_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long offset,
    /* [in] */ float newVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DPRSetFloat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRSetLong_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long offset,
    /* [in] */ long newVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DPRSetLong_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRVarBufWrite_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ VARIANT vData,
    /* [in] */ VARIANT vAddresses,
    /* [in] */ VARIANT vTypes,
    /* [out] */ long *pWritten);


void __RPC_STUB IPmacDevice_DPRVarBufWrite_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRFloat_Proxy( 
    IPmacDevice * This,
    /* [in] */ __int64 d,
    /* [in] */ double scale,
    /* [out] */ double *pVal);


void __RPC_STUB IPmacDevice_DPRFloat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetRomVersion_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out] */ BSTR *pVal);


void __RPC_STUB IPmacDevice_GetRomVersion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetRomDate_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out] */ BSTR *pVal);


void __RPC_STUB IPmacDevice_GetRomDate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetPmacLocation_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out] */ DEVLOCATIONTYPE *pVal);


void __RPC_STUB IPmacDevice_GetPmacLocation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetResponse_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ BSTR question,
    /* [out] */ BSTR *pAnswer);


void __RPC_STUB IPmacDevice_GetResponse_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_SelectDevice_Proxy( 
    IPmacDevice * This,
    /* [in] */ long hWindow,
    /* [out] */ long *pDeviceNumber,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_SelectDevice_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_DPRSize_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacDevice_get_DPRSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_DPRAvailable_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_DPRAvailable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_SetChecksums_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ VARIANT_BOOL bActive);


void __RPC_STUB IPmacDevice_SetChecksums_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetControlResponse_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short question,
    /* [out] */ BSTR *pAnswer);


void __RPC_STUB IPmacDevice_GetControlResponse_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_LastCommError_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacDevice_get_LastCommError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetLastCommErrorStr_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out] */ BSTR *pVal);


void __RPC_STUB IPmacDevice_GetLastCommErrorStr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetMacroIVariableLong_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long node,
    /* [in] */ long num,
    /* [in] */ long def,
    /* [out] */ long *pVal);


void __RPC_STUB IPmacDevice_GetMacroIVariableLong_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_DoChecksums_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_DoChecksums_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacDevice_put_DoChecksums_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IPmacDevice_put_DoChecksums_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetIVariableLong_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long num,
    /* [in] */ long def,
    /* [out] */ long *pVal);


void __RPC_STUB IPmacDevice_GetIVariableLong_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetIVariableDouble_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long num,
    /* [in] */ double def,
    /* [out] */ double *pVal);


void __RPC_STUB IPmacDevice_GetIVariableDouble_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_SetIVariableLong_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long num,
    /* [in] */ long newVal);


void __RPC_STUB IPmacDevice_SetIVariableLong_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_SetIVariableDouble_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long num,
    /* [in] */ double newVal);


void __RPC_STUB IPmacDevice_SetIVariableDouble_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetAsciiComm_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out] */ DEVASCIIMODE *pVal);


void __RPC_STUB IPmacDevice_GetAsciiComm_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_SetAsciiComm_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ DEVASCIIMODE newVal);


void __RPC_STUB IPmacDevice_SetAsciiComm_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetVariableLong_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short ch,
    /* [in] */ long num,
    /* [in] */ long def,
    /* [out] */ long *pVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_GetVariableLong_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetVariableDouble_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short ch,
    /* [in] */ long num,
    /* [in] */ double def,
    /* [out] */ double *pVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_GetVariableDouble_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_SetVariableLong_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short ch,
    /* [in] */ long num,
    /* [in] */ long newVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_SetVariableLong_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_SetVariableDouble_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short ch,
    /* [in] */ long num,
    /* [in] */ double newVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_SetVariableDouble_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_InBootStrapMode_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_InBootStrapMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_MotionBufferOpen_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_MotionBufferOpen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_RotaryBufferOpen_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_RotaryBufferOpen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_IsTurbo_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_IsTurbo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_RestoreLinkList_Proxy( 
    IPmacDevice * This,
    /* [in] */ BSTR firmwareVersion,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_RestoreLinkList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_BackupLinkList_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_BackupLinkList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_SetCriticalIVars_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_SetCriticalIVars_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetLocAndEnum_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out] */ long *pLoc,
    /* [out] */ long *pEnum,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_GetLocAndEnum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetProgramInfo_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ VARIANT_BOOL bPlc,
    /* [in] */ long num,
    /* [out] */ long *pSAdr,
    /* [out] */ long *pFAdr);


void __RPC_STUB IPmacDevice_GetProgramInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DownloadFile_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ BSTR filePath);


void __RPC_STUB IPmacDevice_DownloadFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_AbortDownload_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice);


void __RPC_STUB IPmacDevice_AbortDownload_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DownloadFirmwareFile_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ BSTR filePath,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DownloadFirmwareFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_Download_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ BSTR filePath,
    /* [in] */ VARIANT_BOOL bMacro,
    /* [in] */ VARIANT_BOOL bMap,
    /* [in] */ VARIANT_BOOL bLog,
    /* [in] */ VARIANT_BOOL bDnld,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_Download_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_Downloading_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_Downloading_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_MaxDownloadErrors_Proxy( 
    IPmacDevice * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacDevice_get_MaxDownloadErrors_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacDevice_put_MaxDownloadErrors_Proxy( 
    IPmacDevice * This,
    /* [in] */ long newVal);


void __RPC_STUB IPmacDevice_put_MaxDownloadErrors_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_CompilePLCC_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ BSTR inFilePath,
    /* [in] */ BSTR outFilePath,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_CompilePLCC_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_SendCommand_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ BSTR cmd,
    /* [out] */ long *pStatus);


void __RPC_STUB IPmacDevice_SendCommand_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetPlcStatus_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out] */ long *pNumPlc,
    /* [out] */ long *pNumPlcc,
    /* [out] */ long *pI5);


void __RPC_STUB IPmacDevice_GetPlcStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetFirstPlcc_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out] */ VARIANT_BOOL *pbLoaded,
    /* [out] */ VARIANT_BOOL *pbEnabled,
    /* [out] */ long *pStartAdr,
    /* [out] */ long *pLastAdr,
    /* [out] */ long *pSize,
    /* [out] */ VARIANT_BOOL *pbMore);


void __RPC_STUB IPmacDevice_GetFirstPlcc_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetNextPlcc_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out] */ VARIANT_BOOL *pbLoaded,
    /* [out] */ VARIANT_BOOL *pbEnabled,
    /* [out] */ long *pStartAdr,
    /* [out] */ long *pLastAdr,
    /* [out] */ long *pSize,
    /* [out] */ VARIANT_BOOL *pbMore);


void __RPC_STUB IPmacDevice_GetNextPlcc_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetFirstPlc_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out] */ VARIANT_BOOL *pbEnabled,
    /* [out] */ long *pStartAdr,
    /* [out] */ long *pLastAdr,
    /* [out] */ long *pSize,
    /* [out] */ VARIANT_BOOL *pbMore);


void __RPC_STUB IPmacDevice_GetFirstPlc_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetNextPlc_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out] */ VARIANT_BOOL *pbEnabled,
    /* [out] */ long *pStartAdr,
    /* [out] */ long *pLastAdr,
    /* [out] */ long *pSize,
    /* [out] */ VARIANT_BOOL *pbMore);


void __RPC_STUB IPmacDevice_GetNextPlc_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_IsAnyPlcRunning_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_IsAnyPlcRunning_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetGlobalStatus_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out] */ long *pStatusX,
    /* [out] */ long *pStatusY,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_GetGlobalStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetCoordStatus_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long dwCoord,
    /* [out] */ long *pStatusX,
    /* [out] */ long *pStatusY,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_GetCoordStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetMotorStatus_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long lMotor,
    /* [out] */ long *pStatusX,
    /* [out] */ long *pStatusY,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_GetMotorStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_SysServoError_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_SysServoError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_SysReEntryError_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_SysReEntryError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_SysMemChecksumError_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_SysMemChecksumError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_SysPromChecksumError_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_SysPromChecksumError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_SysInposition_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long dwCoord,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_SysInposition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_SysWarnFError_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long dwCoord,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_SysWarnFError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_SysFatalFError_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long dwCoord,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_SysFatalFError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_SysRuntimeError_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long dwCoord,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_SysRuntimeError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_SysCircleRadError_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long dwCoord,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_SysCircleRadError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_SysAmpFaultError_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long dwCoord,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_SysAmpFaultError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetCommandedPos_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long mtr,
    /* [in] */ double scale,
    /* [out] */ double *pVal);


void __RPC_STUB IPmacDevice_GetCommandedPos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetPosition_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long mtr,
    /* [in] */ double scale,
    /* [out] */ double *pVal);


void __RPC_STUB IPmacDevice_GetPosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetNetActualPosition_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long mtr,
    /* [in] */ double scale,
    /* [out] */ double *pVal);


void __RPC_STUB IPmacDevice_GetNetActualPosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetFollowError_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long mtr,
    /* [in] */ double scale,
    /* [out] */ double *pVal);


void __RPC_STUB IPmacDevice_GetFollowError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetVelocity_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long mtr,
    /* [in] */ double scale,
    /* [out] */ double *pVal);


void __RPC_STUB IPmacDevice_GetVelocity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetMasterPos_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long mtr,
    /* [in] */ double scale,
    /* [out] */ double *pVal);


void __RPC_STUB IPmacDevice_GetMasterPos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetCompensationPos_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long mtr,
    /* [in] */ double scale,
    /* [out] */ double *pVal);


void __RPC_STUB IPmacDevice_GetCompensationPos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetTargetPos_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long mtr,
    /* [in] */ double scale,
    /* [out] */ double *pVal);


void __RPC_STUB IPmacDevice_GetTargetPos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetBiasPos_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long mtr,
    /* [in] */ double scale,
    /* [out] */ double *pVal);


void __RPC_STUB IPmacDevice_GetBiasPos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetProgExecPoint_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long dwCoord,
    /* [out] */ long *pVal);


void __RPC_STUB IPmacDevice_GetProgExecPoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetRotBufFull_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long dwCoord,
    /* [out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_GetRotBufFull_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_ProgramRunning_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long dwCoord,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_ProgramRunning_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_ProgramStepping_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long dwCoord,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_ProgramStepping_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_ProgramContMotion_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long dwCoord,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_ProgramContMotion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_ProgramContRequest_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long dwCoord,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_ProgramContRequest_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_ProgramRemaining_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long dwCoord,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacDevice_get_ProgramRemaining_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_MotionBufOpen_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_MotionBufOpen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_ProgramFeedMode_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long dwCoord,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_ProgramFeedMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_MotorAmpEnabled_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long mtr,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_MotorAmpEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_MotorWarnFError_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long mtr,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_MotorWarnFError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_MotorFatalFError_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long mtr,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_MotorFatalFError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_MotorAmpFault_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long mtr,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_MotorAmpFault_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_MotorOnPositionLimit_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long mtr,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_MotorOnPositionLimit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_MotorHomeComplete_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long mtr,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_MotorHomeComplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_MotorInPosition_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long mtr,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_MotorInPosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_TimRemInMove_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long dwCoord,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacDevice_get_TimRemInMove_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_TimeRemInTATS_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long dwCoord,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacDevice_get_TimeRemInTATS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_MotorDataBlockError_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long mtr,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_MotorDataBlockError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_MotorPhased_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long mtr,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_MotorPhased_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_MotorEnabled_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long mtr,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_MotorEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_MotorHandwheelEnabled_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long mtr,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_MotorHandwheelEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_MotorOpenLoop_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long mtr,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_MotorOpenLoop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_MotorOnNegLimit_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long mtr,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_MotorOnNegLimit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_MotorOnPosLimit_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long mtr,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_MotorOnPosLimit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_SetJogReturn_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long mtr);


void __RPC_STUB IPmacDevice_SetJogReturn_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetWhyMotorNotMoving_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long motor,
    /* [out] */ long *pVal);


void __RPC_STUB IPmacDevice_GetWhyMotorNotMoving_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetWhyMotorNotMovingStr_Proxy( 
    IPmacDevice * This,
    /* [in] */ long err,
    /* [out][in] */ BSTR *pVal);


void __RPC_STUB IPmacDevice_GetWhyMotorNotMovingStr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetWhyCSNotMoving_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long cs,
    /* [out] */ long *pVal);


void __RPC_STUB IPmacDevice_GetWhyCSNotMoving_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetWhyCSNotMovingStr_Proxy( 
    IPmacDevice * This,
    /* [in] */ long err,
    /* [out][in] */ BSTR *pVal);


void __RPC_STUB IPmacDevice_GetWhyCSNotMovingStr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_BufferOpen_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_BufferOpen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_GatherPeriod_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacDevice_get_GatherPeriod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacDevice_put_GatherPeriod_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long newVal);


void __RPC_STUB IPmacDevice_put_GatherPeriod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_GatherEnabled_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long num,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_GatherEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacDevice_put_GatherEnabled_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long num,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IPmacDevice_put_GatherEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_GatherMode_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ DEVGATMODE *pVal);


void __RPC_STUB IPmacDevice_get_GatherMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacDevice_put_GatherMode_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ DEVGATMODE newVal);


void __RPC_STUB IPmacDevice_put_GatherMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_GatherSampleTime_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ double *pVal);


void __RPC_STUB IPmacDevice_get_GatherSampleTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacDevice_put_GatherSampleTime_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ double newVal);


void __RPC_STUB IPmacDevice_put_GatherSampleTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetNumGatherSources_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out] */ long *pVal);


void __RPC_STUB IPmacDevice_GetNumGatherSources_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetNumGatherSamples_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out] */ long *pVal);


void __RPC_STUB IPmacDevice_GetNumGatherSamples_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_SetGather_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long num,
    /* [in] */ BSTR str,
    /* [in] */ VARIANT_BOOL bEnable,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_SetGather_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_SetQuickGather_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long lComMask,
    /* [in] */ long lEncMask,
    /* [in] */ long lDacMask,
    /* [in] */ long lCurMask,
    /* [in] */ VARIANT_BOOL bEnable,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_SetQuickGather_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_SetQuickGatherWithDirectCurrent_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long lComMask,
    /* [in] */ long lEncMask,
    /* [in] */ long lDacMask,
    /* [in] */ long lCurMask,
    /* [in] */ VARIANT_BOOL bEnable,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_SetQuickGatherWithDirectCurrent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetGather_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long num,
    /* [out][in] */ BSTR *pStr,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_GetGather_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_ClearGather_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice);


void __RPC_STUB IPmacDevice_ClearGather_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_InitGather_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long size,
    /* [in] */ double msec,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_InitGather_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_ClearGatherData_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice);


void __RPC_STUB IPmacDevice_ClearGatherData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_CollectGatherData_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out] */ long *pSources,
    /* [out] */ long *pSamples,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_CollectGatherData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetGatherSamples_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long sourceNum,
    /* [out][in] */ VARIANT *pVariant,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_GetGatherSamples_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetGatherPoint_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long sourceNum,
    /* [in] */ long sampleNum,
    /* [out] */ double *pVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_GetGatherPoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_StartGather_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_StartGather_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_StopGather_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice);


void __RPC_STUB IPmacDevice_StopGather_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_InitPlotGather_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long size,
    /* [in] */ long period,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_InitPlotGather_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_ReadGatherFile_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ BSTR bsFilePath,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_ReadGatherFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_WriteGatherFile_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ BSTR bsFilePath,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_WriteGatherFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_strtod48f_Proxy( 
    IPmacDevice * This,
    /* [in] */ BSTR str,
    /* [out] */ double *pVal);


void __RPC_STUB IPmacDevice_strtod48f_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_strtod48l_Proxy( 
    IPmacDevice * This,
    /* [in] */ BSTR str,
    /* [out] */ double *pVal);


void __RPC_STUB IPmacDevice_strtod48l_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_strtod24_Proxy( 
    IPmacDevice * This,
    /* [in] */ BSTR str,
    /* [out] */ double *pVal);


void __RPC_STUB IPmacDevice_strtod24_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_strtod32dp_Proxy( 
    IPmacDevice * This,
    /* [in] */ BSTR str,
    /* [out] */ double *pVal);


void __RPC_STUB IPmacDevice_strtod32dp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_strto32f_Proxy( 
    IPmacDevice * This,
    /* [in] */ BSTR str,
    /* [out] */ double *pVal);


void __RPC_STUB IPmacDevice_strto32f_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_UploadMacroConfig_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ BSTR bsFilePath,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_UploadMacroConfig_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DownloadCurrentFeedbackTest_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short mtr,
    /* [in] */ double pwmDutyCycle,
    /* [in] */ short pwmFreq,
    /* [in] */ VARIANT_BOOL bMacro,
    /* [in] */ VARIANT_BOOL bUltra,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DownloadCurrentFeedbackTest_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_ReadResultsFromCurrentFeedbackTest_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short mtr,
    /* [out] */ long *pVal);


void __RPC_STUB IPmacDevice_ReadResultsFromCurrentFeedbackTest_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DownloadVoltageSixStepTest_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short mtr,
    /* [in] */ double pwmDutyCycle,
    /* [in] */ VARIANT_BOOL bMacro,
    /* [in] */ VARIANT_BOOL bUltra,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DownloadVoltageSixStepTest_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_ReadResultsFromSixStepTest_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ VARIANT_BOOL bSipFirstStep,
    /* [out][in] */ VARIANT *pvStepArray,
    /* [out] */ long *pResult);


void __RPC_STUB IPmacDevice_ReadResultsFromSixStepTest_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DownloadCurrentSixStepTest_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short motor,
    /* [in] */ double pwmDutyCycle,
    /* [in] */ VARIANT_BOOL bMacro,
    /* [in] */ VARIANT_BOOL bUltra,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DownloadCurrentSixStepTest_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DownloadOpenLoopTest_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short motor,
    /* [in] */ short magnitude,
    /* [in] */ short time,
    /* [in] */ short reps,
    /* [in] */ VARIANT_BOOL bGatherCurrents,
    /* [in] */ VARIANT_BOOL bMacro,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DownloadOpenLoopTest_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DownloadOpenLoopTest1_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short motor,
    /* [in] */ short magnitude,
    /* [in] */ short time,
    /* [in] */ short reps,
    /* [in] */ VARIANT_BOOL bGatherCurrents,
    /* [in] */ VARIANT_BOOL bMacro,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DownloadOpenLoopTest1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DownloadVelLimitPLC_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short motor,
    /* [in] */ long maxspd,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DownloadVelLimitPLC_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DownloadPosLimitPLC_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short motor,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DownloadPosLimitPLC_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DownloadUVWSixStepTest_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short motor,
    /* [in] */ double pwmDutyCycle,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DownloadUVWSixStepTest_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_ReadResultsFromUVWSixStepTest_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out][in] */ VARIANT *pvArray,
    /* [out] */ long *pResult);


void __RPC_STUB IPmacDevice_ReadResultsFromUVWSixStepTest_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_SetIVarForFeedback_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short IVar,
    /* [in] */ long address,
    /* [in] */ DEVFEEDBACKTYPE fbType,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_SetIVarForFeedback_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DownloadAndRunRoughPhasePLC_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short motor,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DownloadAndRunRoughPhasePLC_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_CreateIncrementalFinePhasePLC_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ BSTR filePath,
    /* [in] */ short PLCNumber,
    /* [in] */ short motor,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_CreateIncrementalFinePhasePLC_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DoACurrentLoopStep_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short motor,
    /* [in] */ short dwellTime,
    /* [in] */ long magnitude,
    /* [in] */ VARIANT_BOOL bAutoTune,
    /* [out] */ BSTR *pbsCTSCurrent,
    /* [in] */ double pwmDutyCycle,
    /* [in] */ short divisor,
    /* [in] */ VARIANT_BOOL bUltra);


void __RPC_STUB IPmacDevice_DoACurrentLoopStep_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DoACurrentLoopStepBrush_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short motor,
    /* [in] */ short dwellTime,
    /* [in] */ long magnitude,
    /* [in] */ VARIANT_BOOL bAutoTune,
    /* [out] */ BSTR *pbsCTSCurrent,
    /* [in] */ double pwmDutyCycle,
    /* [in] */ short divisor,
    /* [in] */ VARIANT_BOOL bUltra);


void __RPC_STUB IPmacDevice_DoACurrentLoopStepBrush_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DoAPositionLoopStep_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short motor,
    /* [in] */ short stepSize,
    /* [in] */ short stepTime,
    /* [in] */ VARIANT_BOOL bBiDirectional,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DoAPositionLoopStep_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DoAParabolicMove_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short motor,
    /* [in] */ long stepSize,
    /* [in] */ short stepTime,
    /* [in] */ VARIANT_BOOL bBiDirectional,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DoAParabolicMove_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DoAGantryPositionLoopStep_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short motor,
    /* [in] */ short motor2,
    /* [in] */ short stepSize,
    /* [in] */ short stepTime,
    /* [in] */ VARIANT_BOOL bBiDirectional,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DoAGantryPositionLoopStep_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DoAGantryParabolicMove_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short motor,
    /* [in] */ short motor2,
    /* [in] */ long stepSize,
    /* [in] */ short stepTime,
    /* [in] */ VARIANT_BOOL bBiDirectional,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DoAGantryParabolicMove_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DoAGantryParabolic_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short motor,
    /* [in] */ short motor2,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DoAGantryParabolic_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_ConvertWordToStringFloat_Proxy( 
    IPmacDevice * This,
    /* [in] */ BSTR word,
    /* [out] */ BSTR *pVal);


void __RPC_STUB IPmacDevice_ConvertWordToStringFloat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_ConvertWordToShort_Proxy( 
    IPmacDevice * This,
    /* [in] */ BSTR word,
    /* [in] */ short offset,
    /* [out] */ short *pVal);


void __RPC_STUB IPmacDevice_ConvertWordToShort_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_ConvertWordToLong_Proxy( 
    IPmacDevice * This,
    /* [in] */ BSTR word,
    /* [out] */ long *pVal);


void __RPC_STUB IPmacDevice_ConvertWordToLong_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_RestoreModifiedIVars_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short motor);


void __RPC_STUB IPmacDevice_RestoreModifiedIVars_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_CalculateStepStatistics_Proxy( 
    IPmacDevice * This,
    /* [out][in] */ VARIANT *pvData,
    /* [in] */ long stepSize,
    /* [in] */ long I10,
    /* [in] */ long I19,
    /* [out] */ BSTR *pbsRiseTime,
    /* [out] */ BSTR *pbsPeakTime,
    /* [out] */ BSTR *pbsNatFreq,
    /* [out] */ BSTR *pbsOvershoot,
    /* [out] */ BSTR *pbsDamping,
    /* [out] */ BSTR *pbsSettleTime);


void __RPC_STUB IPmacDevice_CalculateStepStatistics_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_LowPassFilterTheData_Proxy( 
    IPmacDevice * This,
    /* [out][in] */ VARIANT *pvData,
    /* [in] */ BSTR bsCurrent);


void __RPC_STUB IPmacDevice_LowPassFilterTheData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DifferentiateTheData_Proxy( 
    IPmacDevice * This,
    /* [out][in] */ VARIANT *pvData,
    /* [in] */ long I10,
    /* [in] */ long I19,
    /* [out] */ long *pMinVal,
    /* [out] */ long *pMaxVal);


void __RPC_STUB IPmacDevice_DifferentiateTheData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DownloadAutoTuningPLC_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short motor,
    /* [in] */ VARIANT_BOOL bCurrentLoop,
    /* [in] */ VARIANT_BOOL bBypassJog,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DownloadAutoTuningPLC_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DownloadGantryAutoTuningPLCP1_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short motor,
    /* [in] */ short motor2,
    /* [in] */ VARIANT_BOOL bCurrentLoop,
    /* [in] */ VARIANT_BOOL bBypassJog,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DownloadGantryAutoTuningPLCP1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DownloadGantryAutoTuningPLC_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short motor,
    /* [in] */ short motor2,
    /* [in] */ VARIANT_BOOL bCurrentLoop,
    /* [in] */ VARIANT_BOOL bBypassJog,
    /* [in] */ double MaxTravel,
    /* [in] */ long time,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DownloadGantryAutoTuningPLC_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_MagEstimation_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ BSTR filePath,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_MagEstimation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_SlopeEstimation_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ BSTR filePath,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_SlopeEstimation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_SetCurrentGather_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long mask,
    /* [in] */ VARIANT_BOOL bEnable,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_SetCurrentGather_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DownloadOpenLoopTestForPMAC1_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short motor,
    /* [in] */ short magnitude,
    /* [in] */ short time,
    /* [in] */ short reps,
    /* [in] */ VARIANT_BOOL bGatherCurrents,
    /* [in] */ VARIANT_BOOL bMacro,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DownloadOpenLoopTestForPMAC1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetMacroNodes_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out] */ long *pNumMacroStations,
    /* [out] */ long *pNumActiveNodes);


void __RPC_STUB IPmacDevice_GetMacroNodes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetMacroNodeInfo_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long modeNum,
    /* [out] */ VARIANT_BOOL *pbEnabled,
    /* [out] */ VARIANT_BOOL *pbIsIONode,
    /* [out] */ long *pID);


void __RPC_STUB IPmacDevice_GetMacroNodeInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_SetMacroIVariableString_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long node,
    /* [in] */ long num,
    /* [in] */ BSTR bsMIVarValue,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_SetMacroIVariableString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetMacroIVariableString_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long node,
    /* [in] */ long num,
    /* [in] */ BSTR bsDefault,
    /* [out] */ BSTR *pbsVal);


void __RPC_STUB IPmacDevice_GetMacroIVariableString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetCommandLogging_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out] */ VARIANT_BOOL *pbON);


void __RPC_STUB IPmacDevice_GetCommandLogging_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_SetCommandLogging_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ VARIANT_BOOL bON,
    /* [in] */ BSTR bsFilePath,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_SetCommandLogging_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetEnumeration_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out] */ long *pVal);


void __RPC_STUB IPmacDevice_GetEnumeration_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetControlValues_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short ctrlCh,
    /* [out][in] */ VARIANT *pVariant,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_GetControlValues_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_MultiDownload_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ BSTR outFile,
    /* [in] */ BSTR iniFile,
    /* [in] */ BSTR userID,
    /* [in] */ VARIANT_BOOL bMacro,
    /* [in] */ VARIANT_BOOL bMap,
    /* [in] */ VARIANT_BOOL bLog,
    /* [in] */ VARIANT_BOOL bDnld,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_MultiDownload_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_AddDownloadFile_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ BSTR iniFile,
    /* [in] */ BSTR userID,
    /* [in] */ BSTR dlFile,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_AddDownloadFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_RemoveDownloadFile_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ BSTR iniFile,
    /* [in] */ BSTR userId,
    /* [in] */ BSTR dlFile,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_RemoveDownloadFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_RenumberFiles_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long fileNumber,
    /* [in] */ BSTR iniFile);


void __RPC_STUB IPmacDevice_RenumberFiles_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetVariableStr_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short ch,
    /* [in] */ long num,
    /* [out] */ BSTR *pVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_GetVariableStr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRVarBufReadAll_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long h,
    /* [out][in] */ VARIANT *pVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DPRVarBufReadAll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_UltraLightAnalyze_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out][in] */ VARIANT *pVal,
    /* [out] */ long *pActiveNodes);


void __RPC_STUB IPmacDevice_UltraLightAnalyze_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetPlcStatusStructure_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out][in] */ VARIANT *pVal,
    /* [out] */ long *pPrograms);


void __RPC_STUB IPmacDevice_GetPlcStatusStructure_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetBitValue_Proxy( 
    IPmacDevice * This,
    /* [in] */ BSTR hexStr,
    /* [in] */ short bit,
    /* [out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_GetBitValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetNumDeviceKeys_Proxy( 
    IPmacDevice * This,
    /* [out] */ long *pVal);


void __RPC_STUB IPmacDevice_GetNumDeviceKeys_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRGetLongDouble_Proxy( 
    IPmacDevice * This,
    /* [in] */ LONG dwDevice,
    /* [in] */ LONG address,
    /* [out] */ DOUBLE *pVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DPRGetLongDouble_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRGetFixedDouble_Proxy( 
    IPmacDevice * This,
    /* [in] */ LONG dwDevice,
    /* [in] */ LONG address,
    /* [out] */ DOUBLE *pVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DPRGetFixedDouble_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRGetShort_Proxy( 
    IPmacDevice * This,
    /* [in] */ LONG dwDevice,
    /* [in] */ LONG address,
    /* [out] */ SHORT *pVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DPRGetShort_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRSetShort_Proxy( 
    IPmacDevice * This,
    /* [in] */ LONG dwDevice,
    /* [in] */ LONG address,
    /* [in] */ SHORT newVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DPRSetShort_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRDWordBitSet_Proxy( 
    IPmacDevice * This,
    /* [in] */ LONG dwDevice,
    /* [in] */ LONG offset,
    /* [in] */ LONG bit,
    /* [out] */ VARIANT_BOOL *pVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DPRDWordBitSet_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRResetDWordBit_Proxy( 
    IPmacDevice * This,
    /* [in] */ LONG dwDevice,
    /* [in] */ LONG offset,
    /* [in] */ LONG bit,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DPRResetDWordBit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRSetDWordBit_Proxy( 
    IPmacDevice * This,
    /* [in] */ LONG dwDevice,
    /* [in] */ LONG offset,
    /* [in] */ LONG bit,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DPRSetDWordBit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetFirmwareType_Proxy( 
    IPmacDevice * This,
    /* [in] */ LONG dwDevice,
    /* [out] */ DEVPMACFIRMWARETYPE *pVal);


void __RPC_STUB IPmacDevice_GetFirmwareType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetResponseEx_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ BSTR question,
    /* [in] */ VARIANT_BOOL bAddLF,
    /* [out] */ BSTR *pAnswer,
    /* [out] */ long *pStatus);


void __RPC_STUB IPmacDevice_GetResponseEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRVarBufWriteEx_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ VARIANT vData,
    /* [out] */ long *pWritten);


void __RPC_STUB IPmacDevice_DPRVarBufWriteEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRGetDWordMask_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long offset,
    /* [in] */ long mask,
    /* [out] */ long *pVal);


void __RPC_STUB IPmacDevice_DPRGetDWordMask_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRSetDWordMask_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long offset,
    /* [in] */ long mask,
    /* [in] */ long Val);


void __RPC_STUB IPmacDevice_DPRSetDWordMask_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRRotBufRemove_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long bufnum);


void __RPC_STUB IPmacDevice_DPRRotBufRemove_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRRotBufInit_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DPRRotBufInit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRRotBufClr_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long bufnum);


void __RPC_STUB IPmacDevice_DPRRotBufClr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRSetRotBuf_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ VARIANT_BOOL on);


void __RPC_STUB IPmacDevice_DPRSetRotBuf_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRAsciiStrToRotEx_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ BSTR inpstr,
    /* [in] */ long bufnum,
    /* [in] */ VARIANT_BOOL bSendRemaining,
    /* [out] */ long *pStatus);


void __RPC_STUB IPmacDevice_DPRAsciiStrToRotEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRAsciiStrToRot_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ BSTR inpstr,
    /* [in] */ long bufnum,
    /* [out] */ long *pStatus);


void __RPC_STUB IPmacDevice_DPRAsciiStrToRot_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetProgramMode_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long dwCoord,
    /* [out] */ DEVPROGRAMMODE *pVal);


void __RPC_STUB IPmacDevice_GetProgramMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetProgramMotionMode_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long dwCoord,
    /* [out] */ DEVMOTIONMODE *pVal);


void __RPC_STUB IPmacDevice_GetProgramMotionMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_MapDeviceNumToEnum_Proxy( 
    IPmacDevice * This,
    /* [in] */ long location,
    /* [in] */ long enumeration,
    /* [out] */ long *pDeviceNumber);


void __RPC_STUB IPmacDevice_MapDeviceNumToEnum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_MapLocAndEnumFromDeviceNum_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out] */ long *pLocation,
    /* [out] */ long *pEnumeration);


void __RPC_STUB IPmacDevice_MapLocAndEnumFromDeviceNum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetFeedRateAndMode_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long dwCoord,
    /* [out] */ double *pRate,
    /* [out] */ VARIANT_BOOL *pMode);


void __RPC_STUB IPmacDevice_GetFeedRateAndMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetMotorMotionMode_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long dwMotor,
    /* [out] */ DEVMOTION *pVal);


void __RPC_STUB IPmacDevice_GetMotorMotionMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_CommandLog_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ BSTR bStr,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_CommandLog_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRGetMem_Proxy( 
    IPmacDevice * This,
    /* [in] */ LONG dwDevice,
    /* [in] */ LONG offset,
    /* [in] */ LONG bytes,
    /* [out][in] */ VARIANT *pVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DPRGetMem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRSetMem_Proxy( 
    IPmacDevice * This,
    /* [in] */ LONG dwDevice,
    /* [in] */ LONG offset,
    /* [in] */ LONG bytes,
    /* [in] */ VARIANT Val,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DPRSetMem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetResponseProgress_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ BSTR question,
    /* [in] */ VARIANT_BOOL bAddLF,
    /* [out] */ BSTR *pAnswer,
    /* [out] */ long *pStatus);


void __RPC_STUB IPmacDevice_GetResponseProgress_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_Abort_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice);


void __RPC_STUB IPmacDevice_Abort_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_InterruptsEnabled_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_InterruptsEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacDevice_put_InterruptsEnabled_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IPmacDevice_put_InterruptsEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_InterruptMask_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacDevice_get_InterruptMask_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacDevice_put_InterruptMask_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long newVal);


void __RPC_STUB IPmacDevice_put_InterruptMask_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_PmacSave_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ BSTR question,
    /* [out] */ long *pStatus);


void __RPC_STUB IPmacDevice_PmacSave_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_PmacReset_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ BSTR question,
    /* [in] */ VARIANT_BOOL bAsciiRingComm,
    /* [out] */ long *pStatus);


void __RPC_STUB IPmacDevice_PmacReset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_PhaseMotor_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ BSTR question,
    /* [out] */ long *pStatuss);


void __RPC_STUB IPmacDevice_PhaseMotor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetControlResponseEx_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short question,
    /* [out] */ BSTR *pAnswer,
    /* [out] */ long *pStatus);


void __RPC_STUB IPmacDevice_GetControlResponseEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_USMonitoringEnabled_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_USMonitoringEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacDevice_put_USMonitoringEnabled_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IPmacDevice_put_USMonitoringEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_USMonitoringPeriod_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacDevice_get_USMonitoringPeriod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacDevice_put_USMonitoringPeriod_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long newVal);


void __RPC_STUB IPmacDevice_put_USMonitoringPeriod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRTest_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DPRTest_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_AbortTest_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice);


void __RPC_STUB IPmacDevice_AbortTest_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRGetMotorRpt_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long motor,
    /* [out][in] */ DEVFGMOTOR *pStruct,
    /* [out] */ long *pServo,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DPRGetMotorRpt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRGetMotorRptTurbo_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long motor,
    /* [out][in] */ DEVFGMOTORTURBO *pStruct,
    /* [out] */ long *pServo,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DPRGetMotorRptTurbo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRGetCoordRpt_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long coord,
    /* [out][in] */ DEVBGCOORD *pStruct,
    /* [out] */ long *pServo,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DPRGetCoordRpt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRGetCoordRptTurbo_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long coord,
    /* [out][in] */ DEVBGCOORDTURBO *pStruct,
    /* [out] */ long *pServo,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DPRGetCoordRptTurbo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_DPRMotorRptEnabled_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_DPRMotorRptEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacDevice_put_DPRMotorRptEnabled_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IPmacDevice_put_DPRMotorRptEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_DPRMotorRptMask_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacDevice_get_DPRMotorRptMask_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacDevice_put_DPRMotorRptMask_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long newVal);


void __RPC_STUB IPmacDevice_put_DPRMotorRptMask_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_DPRMotorRptMSec_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacDevice_get_DPRMotorRptMSec_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacDevice_put_DPRMotorRptMSec_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long newVal);


void __RPC_STUB IPmacDevice_put_DPRMotorRptMSec_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_DPRCoordRptEnabled_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_DPRCoordRptEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacDevice_put_DPRCoordRptEnabled_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IPmacDevice_put_DPRCoordRptEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_DPRCoordRptMSec_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacDevice_get_DPRCoordRptMSec_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacDevice_put_DPRCoordRptMSec_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long newVal);


void __RPC_STUB IPmacDevice_put_DPRCoordRptMSec_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_DPRCoordRptMax_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacDevice_get_DPRCoordRptMax_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacDevice_put_DPRCoordRptMax_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long newVal);


void __RPC_STUB IPmacDevice_put_DPRCoordRptMax_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_UltraLightAnalyzeStruct_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out][in] */ DEVULTRALIGHT_INFO *pStruct,
    /* [out] */ long *pActiveNodes);


void __RPC_STUB IPmacDevice_UltraLightAnalyzeStruct_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetPlcStatusStruct_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out][in] */ DEVTOTAL_PLC_STATUS_STRUCT *pStruct,
    /* [out] */ long *pPrograms);


void __RPC_STUB IPmacDevice_GetPlcStatusStruct_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_DPRTesting_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_DPRTesting_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_RawGetResponseEx_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ BSTR question,
    /* [in] */ VARIANT_BOOL bAddLF,
    /* [out] */ BSTR *pAnswer,
    /* [out] */ long *pStatus);


void __RPC_STUB IPmacDevice_RawGetResponseEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_RawGetControlResponseEx_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ short question,
    /* [out] */ BSTR *pAnswer,
    /* [out] */ long *pStatus);


void __RPC_STUB IPmacDevice_RawGetControlResponseEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_ModBusCommand_Proxy( 
    IPmacDevice * This,
    /* [in] */ LONG dwDevice,
    /* [in] */ BYTE command,
    /* [out] */ VARIANT *counters,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_ModBusCommand_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_ModbusAvailable_Proxy( 
    IPmacDevice * This,
    /* [in] */ LONG dwDevice,
    /* [in] */ BYTE command,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_ModbusAvailable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRSetupBuffers_Proxy( 
    IPmacDevice * This,
    /* [in] */ ULONG dwDevice,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DPRSetupBuffers_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DPRBackground_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long on,
    /* [in] */ long period,
    /* [in] */ long coord,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DPRBackground_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_DataCollectionEnabled_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacDevice_get_DataCollectionEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacDevice_get_DataCollectionResolution_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacDevice_get_DataCollectionResolution_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetTurboMotorStatusEx_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long lMotor,
    /* [out][in] */ DEVMSTURBO *pStatus,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_GetTurboMotorStatusEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetNTMotorStatusEx_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long lMotor,
    /* [out][in] */ DEVMSNONTURBO *pStatus,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_GetNTMotorStatusEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetTurboGlobalStatusEx_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out][in] */ DEVGSTURBO *pStatus,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_GetTurboGlobalStatusEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetNTGlobalStatusEx_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out][in] */ DEVGSNONTURBO *pStatus,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_GetNTGlobalStatusEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetTurboCoordinateStatusEx_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long dwCoord,
    /* [out][in] */ DEVCSTURBO *pStatus,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_GetTurboCoordinateStatusEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_GetNTCoordinateStatusEx_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [in] */ long dwCoord,
    /* [out][in] */ DEVCSNONTURBO *pStatus,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_GetNTCoordinateStatusEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacDevice_DataGather_Proxy( 
    IPmacDevice * This,
    /* [in] */ long dwDevice,
    /* [out][in] */ SAFEARRAY * *pvArray,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacDevice_DataGather_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPmacDevice_INTERFACE_DEFINED__ */


#ifndef ___IPmacDeviceEvents_DISPINTERFACE_DEFINED__
#define ___IPmacDeviceEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IPmacDeviceEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IPmacDeviceEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("D1C5C7C8-5E0C-495e-9AAF-85B146DA5F13")
    _IPmacDeviceEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IPmacDeviceEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IPmacDeviceEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IPmacDeviceEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IPmacDeviceEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IPmacDeviceEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IPmacDeviceEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IPmacDeviceEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IPmacDeviceEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IPmacDeviceEventsVtbl;

    interface _IPmacDeviceEvents
    {
        CONST_VTBL struct _IPmacDeviceEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IPmacDeviceEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IPmacDeviceEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IPmacDeviceEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IPmacDeviceEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IPmacDeviceEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IPmacDeviceEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IPmacDeviceEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IPmacDeviceEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_PmacDevice;

#ifdef __cplusplus

class DECLSPEC_UUID("B544D2BD-354C-456e-9E88-20F71625E7DF")
PmacDevice;
#endif

#ifndef __IPmacNC_INTERFACE_DEFINED__
#define __IPmacNC_INTERFACE_DEFINED__

/* interface IPmacNC */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPmacNC;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0A322163-C2E1-4152-ADED-91740ACE7513")
    IPmacNC : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Open( 
            /* [in] */ long Device,
            /* [in] */ long Control,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Close( 
            /* [in] */ long Control) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OpenFile( 
            /* [in] */ long Control,
            /* [in] */ long dwCoord,
            /* [in] */ BSTR szFilePath,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CloseFile( 
            /* [in] */ long Control,
            /* [in] */ long dwCoord,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Title( 
            /* [in] */ long Control,
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Title( 
            /* [in] */ long Control,
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DeviceNumber( 
            /* [in] */ long Control,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IsOpen( 
            /* [in] */ long Control,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetMachineMode( 
            /* [in] */ long Control,
            /* [in] */ NCMODE mode) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetMachineMode( 
            /* [in] */ long Control,
            /* [out] */ NCMODE *pMode) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Mode( 
            /* [in] */ long Control,
            /* [retval][out] */ NCMODE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Mode( 
            /* [in] */ long Control,
            /* [in] */ NCMODE newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetRegistryBool( 
            /* [in] */ long Control,
            /* [in] */ BSTR section,
            /* [in] */ BSTR name,
            /* [in] */ VARIANT_BOOL def,
            /* [out] */ VARIANT_BOOL *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetRegistryBool( 
            /* [in] */ long Control,
            /* [in] */ BSTR section,
            /* [in] */ BSTR name,
            /* [in] */ VARIANT_BOOL bVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetRegistryLong( 
            /* [in] */ long Control,
            /* [in] */ BSTR section,
            /* [in] */ BSTR name,
            /* [in] */ long def,
            /* [out] */ long *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetRegistryLong( 
            /* [in] */ long Control,
            /* [in] */ BSTR section,
            /* [in] */ BSTR name,
            /* [in] */ long lVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetRegistryString( 
            /* [in] */ long Control,
            /* [in] */ BSTR section,
            /* [in] */ BSTR name,
            /* [in] */ BSTR def,
            /* [out][in] */ BSTR *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetRegistryString( 
            /* [in] */ long Control,
            /* [in] */ BSTR section,
            /* [in] */ BSTR name,
            /* [in] */ BSTR Val,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetRegistryDouble( 
            /* [in] */ long Control,
            /* [in] */ BSTR section,
            /* [in] */ BSTR name,
            /* [in] */ double def,
            /* [out] */ double *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetRegistryDouble( 
            /* [in] */ long Control,
            /* [in] */ BSTR section,
            /* [in] */ BSTR name,
            /* [in] */ double Val,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CoordSystem( 
            /* [in] */ long Control,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CoordSystem( 
            /* [in] */ long Control,
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_NumCoordSystems( 
            /* [in] */ long Control,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SingleBlock( 
            /* [in] */ long Control,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SingleBlock( 
            /* [in] */ long Control,
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_BlockDelete( 
            /* [in] */ long Control,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_BlockDelete( 
            /* [in] */ long Control,
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_OptionalStop( 
            /* [in] */ long Control,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_OptionalStop( 
            /* [in] */ long Control,
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MachineLock( 
            /* [in] */ long Control,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_MachineLock( 
            /* [in] */ long Control,
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DryRun( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_DryRun( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MachineType( 
            /* [in] */ long Control,
            /* [retval][out] */ NCMACHINETYPE *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_JogSelect( 
            /* [in] */ long Control,
            /* [retval][out] */ NCJOGSELECT *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_JogSelect( 
            /* [in] */ long Control,
            /* [in] */ NCJOGSELECT newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SpeedSelect( 
            /* [in] */ long Control,
            /* [retval][out] */ NCSPEEDSELECT *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SpeedSelect( 
            /* [in] */ long Control,
            /* [in] */ NCSPEEDSELECT newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ProgramLoaded( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAxisEnabled( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ short axis,
            /* [out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetProgramName( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ VARIANT_BOOL bMain,
            /* [out][in] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetProgramPath( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ VARIANT_BOOL bMain,
            /* [out][in] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Semaphore( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Semaphore( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetProgramStatus( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ long *pProgID,
            /* [out] */ long *pNumLines,
            /* [out] */ long *pLine,
            /* [out] */ long *pParseLine,
            /* [out] */ long *pRepeat,
            /* [out] */ long *pCount,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetProgramNumber( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetCurrentLabel( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DistanceSelect( 
            /* [in] */ long Control,
            /* [retval][out] */ NCSPEEDSELECT *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_DistanceSelect( 
            /* [in] */ long Control,
            /* [in] */ NCSPEEDSELECT newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetJogStep( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetHandleStep( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetOrigin( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ VARIANT_BOOL bOrginAll,
            /* [in] */ NCADDRESS adr,
            /* [in] */ VARIANT_BOOL bClear,
            /* [in] */ double val,
            /* [in] */ VARIANT_BOOL metric) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ZeroShift( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ VARIANT_BOOL bShiftAll,
            /* [in] */ NCADDRESS adr,
            /* [in] */ VARIANT_BOOL bClear,
            /* [in] */ double val,
            /* [in] */ VARIANT_BOOL bMetric) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetVectorVelocity( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReadMdiBuffer( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long *hDlg,
            /* [in] */ long idControl) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadMdiBuffer( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long *hDlg,
            /* [in] */ long idControl,
            /* [in] */ long nRepeat) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ClearTextBuffer( 
            /* [in] */ long Control,
            /* [in] */ long coord) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetCurrentGValue( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long group,
            /* [out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AxisSelect( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [retval][out] */ NCAXISSELECT *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_AxisSelect( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ NCAXISSELECT newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetInposition( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_BufferMode( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [retval][out] */ NCBUFFERMODE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_BufferMode( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ NCBUFFERMODE newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetProgramMode( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ DEVPROGRAMMODE *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetSelectedBufferMode( 
            /* [in] */ long Control,
            /* [out] */ NCBUFFERMODE *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetSelectedAxisMotionMode( 
            /* [in] */ long Control,
            /* [out] */ DEVMOTIONMODE *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetProgramMotionMode( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ DEVMOTIONMODE *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetBufferOpen( 
            /* [in] */ long Control,
            /* [out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetBufferRemaining( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ long *pRemain,
            /* [out] */ long *pParsed) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SpindleSelect( 
            /* [in] */ long Control,
            /* [retval][out] */ NCSPINDLESELECT *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SpindleSelect( 
            /* [in] */ long Control,
            /* [in] */ NCSPINDLESELECT newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SpindleRPM( 
            /* [in] */ long Control,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SpindleRPM( 
            /* [in] */ long Control,
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SpindleCSS( 
            /* [in] */ long Control,
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SpindleCSS( 
            /* [in] */ long Control,
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SpindleCSSMode( 
            /* [in] */ long Control,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SpindleCSSMode( 
            /* [in] */ long Control,
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SpindleCSSUnits( 
            /* [in] */ long Control,
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SpindleCSSUnits( 
            /* [in] */ long Control,
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetSpindleActRPM( 
            /* [in] */ long Control,
            /* [out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SpindleMaxRPM( 
            /* [in] */ long Control,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SpindleMaxRPM( 
            /* [in] */ long Control,
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SpindleOverride( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SpindleOverride( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FeedrateOverride( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_FeedrateOverride( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RapidOverride( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_RapidOverride( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CoolantSelect( 
            /* [in] */ long Control,
            /* [retval][out] */ NCCLNTSELECT *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CoolantSelect( 
            /* [in] */ long Control,
            /* [in] */ NCCLNTSELECT newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TimebaseMode( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [retval][out] */ NCTIMEBASEMODE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TimebaseMode( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ NCTIMEBASEMODE newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetFeedrate( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ NCFEEDRATEMODE *pMode,
            /* [out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetFeedrate( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ NCFEEDRATEMODE Mode,
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ThreadLead( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ThreadLead( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetNumOfTools( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurrentToolNumber( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CurrentToolNumber( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_NextToolNumber( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_NextToolNumber( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurrentToolHolderNumber( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CurrentToolHolderNumber( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetToolHolderNumber( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long tool,
            /* [in] */ long holder) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetToolHolderNumber( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long tool,
            /* [out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ZeroAllToolOffsets( 
            /* [in] */ long Control,
            /* [in] */ long coord) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetTypeToolOffset( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long tool,
            /* [in] */ BSTR chAxis,
            /* [in] */ NCTOOLOFSTYPE type,
            /* [in] */ VARIANT_BOOL bMetric,
            /* [out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetTypeToolOffset( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long tool,
            /* [in] */ BSTR chAxis,
            /* [in] */ NCTOOLOFSTYPE type,
            /* [in] */ VARIANT_BOOL bMetric,
            /* [in] */ VARIANT_BOOL bConfirm,
            /* [in] */ double newVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ToolInSpindle( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ToolInSpindle( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ToolDirection( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long tool,
            /* [retval][out] */ NCTOOLDIRECTION *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ToolDirection( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long tool,
            /* [in] */ NCTOOLDIRECTION newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SpindleCountsPerRev( 
            /* [in] */ long Control,
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SpindleCountsPerRev( 
            /* [in] */ long Control,
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SpindleGearRatio( 
            /* [in] */ long Control,
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SpindleGearRatio( 
            /* [in] */ long Control,
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SpindleDetect( 
            /* [in] */ long Control,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SpindleDetect( 
            /* [in] */ long Control,
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SpindleAtSpeed( 
            /* [in] */ long Control,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SpindleAtSpeed( 
            /* [in] */ long Control,
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SpindleAtZero( 
            /* [in] */ long Control,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SpindleAtZero( 
            /* [in] */ long Control,
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SpindleFPR( 
            /* [in] */ long Control,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SpindleFPR( 
            /* [in] */ long Control,
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_HomeInit( 
            /* [in] */ long Control,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_HomeInit( 
            /* [in] */ long Control,
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_HomeMotorMask( 
            /* [in] */ long Control,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_HomeMotorMask( 
            /* [in] */ long Control,
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetActiveGCode( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long group,
            /* [out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetActiveGCodeStr( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long group,
            /* [out][in] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetToolOffset( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetCompOffset( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE UpdateAllMotors( 
            /* [in] */ long Control) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetOperatingTime( 
            /* [in] */ long Control,
            /* [out] */ long *pDays,
            /* [out] */ long *pHours,
            /* [out] */ long *pMinutes,
            /* [out] */ long *pSeconds) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetTotalCuttingTime( 
            /* [in] */ long Control,
            /* [out] */ long *pDays,
            /* [out] */ long *pHours,
            /* [out] */ long *pMinutes,
            /* [out] */ long *pSeconds) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetRunningTime( 
            /* [in] */ long Control,
            /* [out] */ long *pDays,
            /* [out] */ long *pHours,
            /* [out] */ long *pMinutes,
            /* [out] */ long *pSeconds) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetPartTime( 
            /* [in] */ long Control,
            /* [out] */ long *pHours,
            /* [out] */ long *pMinutes,
            /* [out] */ long *pSeconds) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetCycleCuttingTime( 
            /* [in] */ long Control,
            /* [out] */ long *pHours,
            /* [out] */ long *pMinutes,
            /* [out] */ long *pSeconds) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetCycleTime( 
            /* [in] */ long Control,
            /* [out] */ long *pHours,
            /* [out] */ long *pMinutes,
            /* [out] */ long *Seconds) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PartsRequired( 
            /* [in] */ long Control,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_PartsRequired( 
            /* [in] */ long Control,
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PartsCount( 
            /* [in] */ long Control,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_PartsCount( 
            /* [in] */ long Control,
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PartsTotal( 
            /* [in] */ long Control,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_PartsTotal( 
            /* [in] */ long Control,
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetNumDisplayedAxis( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetNumDisplayAxisTotal( 
            /* [in] */ long Control,
            /* [out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetMotorPosStr( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long motor,
            /* [in] */ NCPOSTYPE t,
            /* [in] */ VARIANT_BOOL bCombined,
            /* [out][in] */ BSTR *pAdr,
            /* [out][in] */ BSTR *pPos,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IsSpindle( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long motor,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_IsSpindle( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long motor,
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IsDisplayed( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long motor,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_IsDisplayed( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long motor,
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetMotorUnitsStr( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long motor,
            /* [out][in] */ BSTR *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetProgramLines( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long start,
            /* [in] */ long count,
            /* [out][in] */ VARIANT *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetProgramLine( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long lineNumber,
            /* [out] */ BSTR *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetInputLong( 
            /* [in] */ long Control,
            /* [in] */ long num,
            /* [out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetOutputLong( 
            /* [in] */ long Control,
            /* [in] */ long num,
            /* [out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetCommandLong( 
            /* [in] */ long Control,
            /* [in] */ long num,
            /* [out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetStatusLong( 
            /* [in] */ long Control,
            /* [in] */ long num,
            /* [out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_HomeInProgress( 
            /* [in] */ long Control,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SpindleCmdRPM( 
            /* [in] */ long Control,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Command( 
            /* [in] */ long Control,
            /* [in] */ long id,
            /* [in] */ long iv,
            /* [in] */ double fv,
            /* [in] */ BSTR bstr,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddErrorRecord( 
            /* [in] */ long Control,
            /* [in] */ long errnum,
            /* [in] */ NCERRORMODE em,
            /* [in] */ NCERRORTYPE et,
            /* [in] */ long line,
            /* [in] */ short ax,
            /* [in] */ BSTR msg) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetParametricDouble( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ BSTR region,
            /* [in] */ long index,
            /* [out] */ double *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetParametricDouble( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ BSTR region,
            /* [in] */ long index,
            /* [in] */ double newVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetSelectedAxisChar( 
            /* [in] */ long Control,
            /* [out] */ short *pAxChar) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurrentErrorLevel( 
            /* [in] */ long Control,
            /* [retval][out] */ NCERRORMODE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CurrentErrorLevel( 
            /* [in] */ long Control,
            /* [in] */ NCERRORMODE newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAxisFormat( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ short ax,
            /* [in] */ VARIANT_BOOL bMetric,
            /* [out] */ long *pLen,
            /* [out] */ long *pDecPlaces) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAxisFormatStr( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ short ax,
            /* [in] */ VARIANT_BOOL bMetric,
            /* [out][in] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetAxisFormat( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ short ax,
            /* [in] */ VARIANT_BOOL bMetric,
            /* [in] */ long len,
            /* [in] */ long decPlaces) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetFirstError( 
            /* [in] */ long Control,
            /* [out][in] */ BSTR *pHeader,
            /* [out][in] */ BSTR *pMessage,
            /* [out][in] */ BSTR *pCode,
            /* [out][in] */ BSTR *pFilename,
            /* [out] */ long *pCount,
            /* [out] */ long *pChanged) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetNextError( 
            /* [in] */ long Control,
            /* [out][in] */ BSTR *pHeader,
            /* [out][in] */ BSTR *pMessage,
            /* [out][in] */ BSTR *pCode,
            /* [out][in] */ BSTR *pFilename,
            /* [out] */ VARIANT_BOOL *pbMore) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OffsetsUpdateClear( 
            /* [in] */ long Control,
            /* [in] */ long clearMask) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OffsetsUpdate( 
            /* [in] */ long Control,
            /* [out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReadErrorMsgBoxText( 
            /* [in] */ long Control,
            /* [in] */ long index,
            /* [out] */ long *pCount,
            /* [out][in] */ BSTR *pText,
            /* [out][in] */ BSTR *pTitle) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ResetMsgBoxError( 
            /* [in] */ long Control,
            /* [in] */ long errRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveParametricVars( 
            /* [in] */ long Control) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadParametricVars( 
            /* [in] */ long Control) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ResetCycleTime( 
            /* [in] */ long Control) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetToolGuageOffset( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long tool,
            /* [in] */ VARIANT_BOOL bMulti,
            /* [in] */ short ax,
            /* [in] */ VARIANT_BOOL bMetric,
            /* [out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetToolGuageOffset( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long tool,
            /* [in] */ VARIANT_BOOL bMulti,
            /* [in] */ short ax,
            /* [in] */ VARIANT_BOOL bMetric,
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AutoSetToolOffset( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long tool,
            /* [in] */ short axis) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetOffsetValue( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ short axis,
            /* [in] */ NCCOORDTYPE t,
            /* [in] */ VARIANT_BOOL bMetric,
            /* [out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetOffsetValue( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ short axis,
            /* [in] */ NCCOORDTYPE t,
            /* [in] */ VARIANT_BOOL bMetric,
            /* [in] */ VARIANT_BOOL bConfirm,
            /* [in] */ double NewVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AutoSetWorkOffset( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ short axis,
            /* [in] */ NCCOORDTYPE t,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SearchText( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ BSTR text,
            /* [in] */ VARIANT_BOOL bMatchCase,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SearchTextNext( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ BSTR text,
            /* [in] */ VARIANT_BOOL bMatchCase,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetBufferText( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ VARIANT_BOOL bMDI,
            /* [out][in] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetBufferText( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long repeat,
            /* [in] */ VARIANT_BOOL bMDI,
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetProcessorClocks( 
            /* [out] */ double *pIO,
            /* [out] */ double *pLA,
            /* [out] */ double *pParse,
            /* [out] */ double *pFormat,
            /* [out] */ double *pWrite) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurrentErrorCount( 
            /* [in] */ long Control,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_BlocksPerSecond( 
            /* [in] */ long Control,
            /* [in] */ long Coord,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetMetricInput( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetToolInfo( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ long *pCurrentTool,
            /* [out] */ long *pNextTool,
            /* [out] */ long *pHolder,
            /* [out] */ long *pHolderTool) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetToolData( 
            /* [in] */ LONG Control,
            /* [in] */ LONG Coord,
            /* [in] */ LONG Tool,
            /* [out][in] */ VARIANT *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ZeroAllTypeToolOffsets( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ NCTOOLOFSTYPE type) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SearchLine( 
            /* [in] */ long Control,
            /* [in] */ long Coord,
            /* [in] */ long Line,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAxisMotor( 
            /* [in] */ long Control,
            /* [in] */ long Coord,
            /* [in] */ short Axis,
            /* [out] */ long *pMotor,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetMotorAxis( 
            /* [in] */ long Control,
            /* [in] */ long Coord,
            /* [in] */ long Motor,
            /* [out] */ short *pAxis,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetMotorPos( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long motor,
            /* [in] */ NCPOSTYPE t,
            /* [out] */ BSTR *pAdr,
            /* [out] */ double *pPos,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAllAxisPos( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ NCPOSTYPE t,
            /* [out][in] */ SAFEARRAY * *pvAdrArray,
            /* [out][in] */ SAFEARRAY * *pvPosArray,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAllAxisPosStr( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ NCPOSTYPE t,
            /* [in] */ VARIANT_BOOL bCoord,
            /* [out][in] */ SAFEARRAY * *pvArray,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetMaxFeedrates( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ double *pMaxPerRev,
            /* [out] */ double *pMaxPerMinute) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetMaxFeedrates( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ double MaxPerRev,
            /* [in] */ double MaxPerMinute) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ClearOffsetValue( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ short axis,
            /* [in] */ NCCOORDTYPE t,
            /* [in] */ VARIANT_BOOL bMetric,
            /* [in] */ double NewVal,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetToolDataStruct( 
            /* [in] */ LONG Control,
            /* [in] */ LONG Coord,
            /* [in] */ LONG Tool,
            /* [out][in] */ NCTOOL *pStruct) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartDataCollection( 
            /* [in] */ long Control,
            /* [in] */ long CollectionMSec,
            /* [in] */ long BufferSize,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StopDataCollection( 
            /* [in] */ long Control) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DataCollect( 
            /* [in] */ long Control,
            /* [in] */ long Coord,
            /* [out][in] */ SAFEARRAY * *pvArray,
            /* [out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ResetPartsCount( 
            /* [in] */ long Control) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ResetPartsTotal( 
            /* [in] */ long Control) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetWorkOffset( 
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ NCCOORDTYPE *t,
            /* [out] */ long *pExtendedVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DataCollectionEnabled( 
            /* [in] */ long Control,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DataCollectionResolution( 
            /* [in] */ long Control,
            /* [retval][out] */ long *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPmacNCVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPmacNC * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPmacNC * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPmacNC * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPmacNC * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPmacNC * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPmacNC * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPmacNC * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Open )( 
            IPmacNC * This,
            /* [in] */ long Device,
            /* [in] */ long Control,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Close )( 
            IPmacNC * This,
            /* [in] */ long Control);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenFile )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long dwCoord,
            /* [in] */ BSTR szFilePath,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CloseFile )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long dwCoord,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Title )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Title )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DeviceNumber )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsOpen )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetMachineMode )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ NCMODE mode);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetMachineMode )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [out] */ NCMODE *pMode);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Mode )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ NCMODE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Mode )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ NCMODE newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetRegistryBool )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ BSTR section,
            /* [in] */ BSTR name,
            /* [in] */ VARIANT_BOOL def,
            /* [out] */ VARIANT_BOOL *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetRegistryBool )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ BSTR section,
            /* [in] */ BSTR name,
            /* [in] */ VARIANT_BOOL bVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetRegistryLong )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ BSTR section,
            /* [in] */ BSTR name,
            /* [in] */ long def,
            /* [out] */ long *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetRegistryLong )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ BSTR section,
            /* [in] */ BSTR name,
            /* [in] */ long lVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetRegistryString )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ BSTR section,
            /* [in] */ BSTR name,
            /* [in] */ BSTR def,
            /* [out][in] */ BSTR *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetRegistryString )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ BSTR section,
            /* [in] */ BSTR name,
            /* [in] */ BSTR Val,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetRegistryDouble )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ BSTR section,
            /* [in] */ BSTR name,
            /* [in] */ double def,
            /* [out] */ double *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetRegistryDouble )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ BSTR section,
            /* [in] */ BSTR name,
            /* [in] */ double Val,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CoordSystem )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CoordSystem )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_NumCoordSystems )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SingleBlock )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SingleBlock )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BlockDelete )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_BlockDelete )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_OptionalStop )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_OptionalStop )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MachineLock )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_MachineLock )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DryRun )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DryRun )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MachineType )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ NCMACHINETYPE *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_JogSelect )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ NCJOGSELECT *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_JogSelect )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ NCJOGSELECT newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SpeedSelect )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ NCSPEEDSELECT *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SpeedSelect )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ NCSPEEDSELECT newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ProgramLoaded )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAxisEnabled )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ short axis,
            /* [out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetProgramName )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ VARIANT_BOOL bMain,
            /* [out][in] */ BSTR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetProgramPath )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ VARIANT_BOOL bMain,
            /* [out][in] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Semaphore )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Semaphore )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetProgramStatus )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ long *pProgID,
            /* [out] */ long *pNumLines,
            /* [out] */ long *pLine,
            /* [out] */ long *pParseLine,
            /* [out] */ long *pRepeat,
            /* [out] */ long *pCount,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetProgramNumber )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetCurrentLabel )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DistanceSelect )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ NCSPEEDSELECT *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DistanceSelect )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ NCSPEEDSELECT newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetJogStep )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ double *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetHandleStep )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ double *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetOrigin )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ VARIANT_BOOL bOrginAll,
            /* [in] */ NCADDRESS adr,
            /* [in] */ VARIANT_BOOL bClear,
            /* [in] */ double val,
            /* [in] */ VARIANT_BOOL metric);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ZeroShift )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ VARIANT_BOOL bShiftAll,
            /* [in] */ NCADDRESS adr,
            /* [in] */ VARIANT_BOOL bClear,
            /* [in] */ double val,
            /* [in] */ VARIANT_BOOL bMetric);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetVectorVelocity )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ double *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReadMdiBuffer )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long *hDlg,
            /* [in] */ long idControl);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadMdiBuffer )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long *hDlg,
            /* [in] */ long idControl,
            /* [in] */ long nRepeat);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ClearTextBuffer )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetCurrentGValue )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long group,
            /* [out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AxisSelect )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [retval][out] */ NCAXISSELECT *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AxisSelect )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ NCAXISSELECT newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetInposition )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BufferMode )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [retval][out] */ NCBUFFERMODE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_BufferMode )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ NCBUFFERMODE newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetProgramMode )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ DEVPROGRAMMODE *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSelectedBufferMode )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [out] */ NCBUFFERMODE *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSelectedAxisMotionMode )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [out] */ DEVMOTIONMODE *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetProgramMotionMode )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ DEVMOTIONMODE *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetBufferOpen )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetBufferRemaining )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ long *pRemain,
            /* [out] */ long *pParsed);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SpindleSelect )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ NCSPINDLESELECT *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SpindleSelect )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ NCSPINDLESELECT newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SpindleRPM )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SpindleRPM )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SpindleCSS )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SpindleCSS )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SpindleCSSMode )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SpindleCSSMode )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SpindleCSSUnits )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SpindleCSSUnits )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ double newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSpindleActRPM )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SpindleMaxRPM )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SpindleMaxRPM )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SpindleOverride )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SpindleOverride )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FeedrateOverride )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_FeedrateOverride )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RapidOverride )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RapidOverride )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CoolantSelect )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ NCCLNTSELECT *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CoolantSelect )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ NCCLNTSELECT newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TimebaseMode )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [retval][out] */ NCTIMEBASEMODE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TimebaseMode )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ NCTIMEBASEMODE newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetFeedrate )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ NCFEEDRATEMODE *pMode,
            /* [out] */ double *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetFeedrate )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ NCFEEDRATEMODE Mode,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ThreadLead )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ThreadLead )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ double newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetNumOfTools )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CurrentToolNumber )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CurrentToolNumber )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_NextToolNumber )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_NextToolNumber )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CurrentToolHolderNumber )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CurrentToolHolderNumber )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetToolHolderNumber )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long tool,
            /* [in] */ long holder);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetToolHolderNumber )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long tool,
            /* [out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ZeroAllToolOffsets )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetTypeToolOffset )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long tool,
            /* [in] */ BSTR chAxis,
            /* [in] */ NCTOOLOFSTYPE type,
            /* [in] */ VARIANT_BOOL bMetric,
            /* [out] */ double *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetTypeToolOffset )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long tool,
            /* [in] */ BSTR chAxis,
            /* [in] */ NCTOOLOFSTYPE type,
            /* [in] */ VARIANT_BOOL bMetric,
            /* [in] */ VARIANT_BOOL bConfirm,
            /* [in] */ double newVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ToolInSpindle )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ToolInSpindle )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ToolDirection )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long tool,
            /* [retval][out] */ NCTOOLDIRECTION *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ToolDirection )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long tool,
            /* [in] */ NCTOOLDIRECTION newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SpindleCountsPerRev )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SpindleCountsPerRev )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SpindleGearRatio )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SpindleGearRatio )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SpindleDetect )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SpindleDetect )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SpindleAtSpeed )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SpindleAtSpeed )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SpindleAtZero )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SpindleAtZero )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SpindleFPR )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SpindleFPR )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HomeInit )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_HomeInit )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HomeMotorMask )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_HomeMotorMask )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetActiveGCode )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long group,
            /* [out] */ double *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetActiveGCodeStr )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long group,
            /* [out][in] */ BSTR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetToolOffset )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetCompOffset )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *UpdateAllMotors )( 
            IPmacNC * This,
            /* [in] */ long Control);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetOperatingTime )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [out] */ long *pDays,
            /* [out] */ long *pHours,
            /* [out] */ long *pMinutes,
            /* [out] */ long *pSeconds);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetTotalCuttingTime )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [out] */ long *pDays,
            /* [out] */ long *pHours,
            /* [out] */ long *pMinutes,
            /* [out] */ long *pSeconds);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetRunningTime )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [out] */ long *pDays,
            /* [out] */ long *pHours,
            /* [out] */ long *pMinutes,
            /* [out] */ long *pSeconds);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetPartTime )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [out] */ long *pHours,
            /* [out] */ long *pMinutes,
            /* [out] */ long *pSeconds);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetCycleCuttingTime )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [out] */ long *pHours,
            /* [out] */ long *pMinutes,
            /* [out] */ long *pSeconds);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetCycleTime )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [out] */ long *pHours,
            /* [out] */ long *pMinutes,
            /* [out] */ long *Seconds);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PartsRequired )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_PartsRequired )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PartsCount )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_PartsCount )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PartsTotal )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_PartsTotal )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetNumDisplayedAxis )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetNumDisplayAxisTotal )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetMotorPosStr )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long motor,
            /* [in] */ NCPOSTYPE t,
            /* [in] */ VARIANT_BOOL bCombined,
            /* [out][in] */ BSTR *pAdr,
            /* [out][in] */ BSTR *pPos,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsSpindle )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long motor,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_IsSpindle )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long motor,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsDisplayed )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long motor,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_IsDisplayed )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long motor,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetMotorUnitsStr )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long motor,
            /* [out][in] */ BSTR *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetProgramLines )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long start,
            /* [in] */ long count,
            /* [out][in] */ VARIANT *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetProgramLine )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long lineNumber,
            /* [out] */ BSTR *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetInputLong )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long num,
            /* [out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetOutputLong )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long num,
            /* [out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetCommandLong )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long num,
            /* [out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetStatusLong )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long num,
            /* [out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HomeInProgress )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SpindleCmdRPM )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Command )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long id,
            /* [in] */ long iv,
            /* [in] */ double fv,
            /* [in] */ BSTR bstr,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddErrorRecord )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long errnum,
            /* [in] */ NCERRORMODE em,
            /* [in] */ NCERRORTYPE et,
            /* [in] */ long line,
            /* [in] */ short ax,
            /* [in] */ BSTR msg);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetParametricDouble )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ BSTR region,
            /* [in] */ long index,
            /* [out] */ double *pVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetParametricDouble )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ BSTR region,
            /* [in] */ long index,
            /* [in] */ double newVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSelectedAxisChar )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [out] */ short *pAxChar);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CurrentErrorLevel )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ NCERRORMODE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CurrentErrorLevel )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ NCERRORMODE newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAxisFormat )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ short ax,
            /* [in] */ VARIANT_BOOL bMetric,
            /* [out] */ long *pLen,
            /* [out] */ long *pDecPlaces);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAxisFormatStr )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ short ax,
            /* [in] */ VARIANT_BOOL bMetric,
            /* [out][in] */ BSTR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetAxisFormat )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ short ax,
            /* [in] */ VARIANT_BOOL bMetric,
            /* [in] */ long len,
            /* [in] */ long decPlaces);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetFirstError )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [out][in] */ BSTR *pHeader,
            /* [out][in] */ BSTR *pMessage,
            /* [out][in] */ BSTR *pCode,
            /* [out][in] */ BSTR *pFilename,
            /* [out] */ long *pCount,
            /* [out] */ long *pChanged);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetNextError )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [out][in] */ BSTR *pHeader,
            /* [out][in] */ BSTR *pMessage,
            /* [out][in] */ BSTR *pCode,
            /* [out][in] */ BSTR *pFilename,
            /* [out] */ VARIANT_BOOL *pbMore);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OffsetsUpdateClear )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long clearMask);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OffsetsUpdate )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReadErrorMsgBoxText )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long index,
            /* [out] */ long *pCount,
            /* [out][in] */ BSTR *pText,
            /* [out][in] */ BSTR *pTitle);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ResetMsgBoxError )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long errRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SaveParametricVars )( 
            IPmacNC * This,
            /* [in] */ long Control);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadParametricVars )( 
            IPmacNC * This,
            /* [in] */ long Control);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ResetCycleTime )( 
            IPmacNC * This,
            /* [in] */ long Control);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetToolGuageOffset )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long tool,
            /* [in] */ VARIANT_BOOL bMulti,
            /* [in] */ short ax,
            /* [in] */ VARIANT_BOOL bMetric,
            /* [out] */ double *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetToolGuageOffset )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long tool,
            /* [in] */ VARIANT_BOOL bMulti,
            /* [in] */ short ax,
            /* [in] */ VARIANT_BOOL bMetric,
            /* [in] */ double newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AutoSetToolOffset )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long tool,
            /* [in] */ short axis);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetOffsetValue )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ short axis,
            /* [in] */ NCCOORDTYPE t,
            /* [in] */ VARIANT_BOOL bMetric,
            /* [out] */ double *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetOffsetValue )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ short axis,
            /* [in] */ NCCOORDTYPE t,
            /* [in] */ VARIANT_BOOL bMetric,
            /* [in] */ VARIANT_BOOL bConfirm,
            /* [in] */ double NewVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AutoSetWorkOffset )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ short axis,
            /* [in] */ NCCOORDTYPE t,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SearchText )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ BSTR text,
            /* [in] */ VARIANT_BOOL bMatchCase,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SearchTextNext )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ BSTR text,
            /* [in] */ VARIANT_BOOL bMatchCase,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetBufferText )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ VARIANT_BOOL bMDI,
            /* [out][in] */ BSTR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetBufferText )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long repeat,
            /* [in] */ VARIANT_BOOL bMDI,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetProcessorClocks )( 
            IPmacNC * This,
            /* [out] */ double *pIO,
            /* [out] */ double *pLA,
            /* [out] */ double *pParse,
            /* [out] */ double *pFormat,
            /* [out] */ double *pWrite);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CurrentErrorCount )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BlocksPerSecond )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long Coord,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetMetricInput )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetToolInfo )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ long *pCurrentTool,
            /* [out] */ long *pNextTool,
            /* [out] */ long *pHolder,
            /* [out] */ long *pHolderTool);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetToolData )( 
            IPmacNC * This,
            /* [in] */ LONG Control,
            /* [in] */ LONG Coord,
            /* [in] */ LONG Tool,
            /* [out][in] */ VARIANT *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ZeroAllTypeToolOffsets )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ NCTOOLOFSTYPE type);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SearchLine )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long Coord,
            /* [in] */ long Line,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAxisMotor )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long Coord,
            /* [in] */ short Axis,
            /* [out] */ long *pMotor,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetMotorAxis )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long Coord,
            /* [in] */ long Motor,
            /* [out] */ short *pAxis,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetMotorPos )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ long motor,
            /* [in] */ NCPOSTYPE t,
            /* [out] */ BSTR *pAdr,
            /* [out] */ double *pPos,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAllAxisPos )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ NCPOSTYPE t,
            /* [out][in] */ SAFEARRAY * *pvAdrArray,
            /* [out][in] */ SAFEARRAY * *pvPosArray,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAllAxisPosStr )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ NCPOSTYPE t,
            /* [in] */ VARIANT_BOOL bCoord,
            /* [out][in] */ SAFEARRAY * *pvArray,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetMaxFeedrates )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ double *pMaxPerRev,
            /* [out] */ double *pMaxPerMinute);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetMaxFeedrates )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ double MaxPerRev,
            /* [in] */ double MaxPerMinute);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ClearOffsetValue )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [in] */ short axis,
            /* [in] */ NCCOORDTYPE t,
            /* [in] */ VARIANT_BOOL bMetric,
            /* [in] */ double NewVal,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetToolDataStruct )( 
            IPmacNC * This,
            /* [in] */ LONG Control,
            /* [in] */ LONG Coord,
            /* [in] */ LONG Tool,
            /* [out][in] */ NCTOOL *pStruct);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StartDataCollection )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long CollectionMSec,
            /* [in] */ long BufferSize,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StopDataCollection )( 
            IPmacNC * This,
            /* [in] */ long Control);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DataCollect )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long Coord,
            /* [out][in] */ SAFEARRAY * *pvArray,
            /* [out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ResetPartsCount )( 
            IPmacNC * This,
            /* [in] */ long Control);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ResetPartsTotal )( 
            IPmacNC * This,
            /* [in] */ long Control);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetWorkOffset )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [in] */ long coord,
            /* [out] */ NCCOORDTYPE *t,
            /* [out] */ long *pExtendedVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DataCollectionEnabled )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DataCollectionResolution )( 
            IPmacNC * This,
            /* [in] */ long Control,
            /* [retval][out] */ long *pVal);
        
        END_INTERFACE
    } IPmacNCVtbl;

    interface IPmacNC
    {
        CONST_VTBL struct IPmacNCVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPmacNC_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPmacNC_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPmacNC_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPmacNC_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPmacNC_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPmacNC_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPmacNC_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPmacNC_Open(This,Device,Control,pbSuccess)	\
    (This)->lpVtbl -> Open(This,Device,Control,pbSuccess)

#define IPmacNC_Close(This,Control)	\
    (This)->lpVtbl -> Close(This,Control)

#define IPmacNC_OpenFile(This,Control,dwCoord,szFilePath,pbSuccess)	\
    (This)->lpVtbl -> OpenFile(This,Control,dwCoord,szFilePath,pbSuccess)

#define IPmacNC_CloseFile(This,Control,dwCoord,pbSuccess)	\
    (This)->lpVtbl -> CloseFile(This,Control,dwCoord,pbSuccess)

#define IPmacNC_get_Title(This,Control,pVal)	\
    (This)->lpVtbl -> get_Title(This,Control,pVal)

#define IPmacNC_put_Title(This,Control,newVal)	\
    (This)->lpVtbl -> put_Title(This,Control,newVal)

#define IPmacNC_get_DeviceNumber(This,Control,pVal)	\
    (This)->lpVtbl -> get_DeviceNumber(This,Control,pVal)

#define IPmacNC_get_IsOpen(This,Control,pVal)	\
    (This)->lpVtbl -> get_IsOpen(This,Control,pVal)

#define IPmacNC_SetMachineMode(This,Control,mode)	\
    (This)->lpVtbl -> SetMachineMode(This,Control,mode)

#define IPmacNC_GetMachineMode(This,Control,pMode)	\
    (This)->lpVtbl -> GetMachineMode(This,Control,pMode)

#define IPmacNC_get_Mode(This,Control,pVal)	\
    (This)->lpVtbl -> get_Mode(This,Control,pVal)

#define IPmacNC_put_Mode(This,Control,newVal)	\
    (This)->lpVtbl -> put_Mode(This,Control,newVal)

#define IPmacNC_GetRegistryBool(This,Control,section,name,def,pVal,pbSuccess)	\
    (This)->lpVtbl -> GetRegistryBool(This,Control,section,name,def,pVal,pbSuccess)

#define IPmacNC_SetRegistryBool(This,Control,section,name,bVal,pbSuccess)	\
    (This)->lpVtbl -> SetRegistryBool(This,Control,section,name,bVal,pbSuccess)

#define IPmacNC_GetRegistryLong(This,Control,section,name,def,pVal,pbSuccess)	\
    (This)->lpVtbl -> GetRegistryLong(This,Control,section,name,def,pVal,pbSuccess)

#define IPmacNC_SetRegistryLong(This,Control,section,name,lVal,pbSuccess)	\
    (This)->lpVtbl -> SetRegistryLong(This,Control,section,name,lVal,pbSuccess)

#define IPmacNC_GetRegistryString(This,Control,section,name,def,pVal,pbSuccess)	\
    (This)->lpVtbl -> GetRegistryString(This,Control,section,name,def,pVal,pbSuccess)

#define IPmacNC_SetRegistryString(This,Control,section,name,Val,pbSuccess)	\
    (This)->lpVtbl -> SetRegistryString(This,Control,section,name,Val,pbSuccess)

#define IPmacNC_GetRegistryDouble(This,Control,section,name,def,pVal,pbSuccess)	\
    (This)->lpVtbl -> GetRegistryDouble(This,Control,section,name,def,pVal,pbSuccess)

#define IPmacNC_SetRegistryDouble(This,Control,section,name,Val,pbSuccess)	\
    (This)->lpVtbl -> SetRegistryDouble(This,Control,section,name,Val,pbSuccess)

#define IPmacNC_get_CoordSystem(This,Control,pVal)	\
    (This)->lpVtbl -> get_CoordSystem(This,Control,pVal)

#define IPmacNC_put_CoordSystem(This,Control,newVal)	\
    (This)->lpVtbl -> put_CoordSystem(This,Control,newVal)

#define IPmacNC_get_NumCoordSystems(This,Control,pVal)	\
    (This)->lpVtbl -> get_NumCoordSystems(This,Control,pVal)

#define IPmacNC_get_SingleBlock(This,Control,pVal)	\
    (This)->lpVtbl -> get_SingleBlock(This,Control,pVal)

#define IPmacNC_put_SingleBlock(This,Control,newVal)	\
    (This)->lpVtbl -> put_SingleBlock(This,Control,newVal)

#define IPmacNC_get_BlockDelete(This,Control,pVal)	\
    (This)->lpVtbl -> get_BlockDelete(This,Control,pVal)

#define IPmacNC_put_BlockDelete(This,Control,newVal)	\
    (This)->lpVtbl -> put_BlockDelete(This,Control,newVal)

#define IPmacNC_get_OptionalStop(This,Control,pVal)	\
    (This)->lpVtbl -> get_OptionalStop(This,Control,pVal)

#define IPmacNC_put_OptionalStop(This,Control,newVal)	\
    (This)->lpVtbl -> put_OptionalStop(This,Control,newVal)

#define IPmacNC_get_MachineLock(This,Control,pVal)	\
    (This)->lpVtbl -> get_MachineLock(This,Control,pVal)

#define IPmacNC_put_MachineLock(This,Control,newVal)	\
    (This)->lpVtbl -> put_MachineLock(This,Control,newVal)

#define IPmacNC_get_DryRun(This,Control,coord,pVal)	\
    (This)->lpVtbl -> get_DryRun(This,Control,coord,pVal)

#define IPmacNC_put_DryRun(This,Control,coord,newVal)	\
    (This)->lpVtbl -> put_DryRun(This,Control,coord,newVal)

#define IPmacNC_get_MachineType(This,Control,pVal)	\
    (This)->lpVtbl -> get_MachineType(This,Control,pVal)

#define IPmacNC_get_JogSelect(This,Control,pVal)	\
    (This)->lpVtbl -> get_JogSelect(This,Control,pVal)

#define IPmacNC_put_JogSelect(This,Control,newVal)	\
    (This)->lpVtbl -> put_JogSelect(This,Control,newVal)

#define IPmacNC_get_SpeedSelect(This,Control,pVal)	\
    (This)->lpVtbl -> get_SpeedSelect(This,Control,pVal)

#define IPmacNC_put_SpeedSelect(This,Control,newVal)	\
    (This)->lpVtbl -> put_SpeedSelect(This,Control,newVal)

#define IPmacNC_get_ProgramLoaded(This,Control,coord,pVal)	\
    (This)->lpVtbl -> get_ProgramLoaded(This,Control,coord,pVal)

#define IPmacNC_GetAxisEnabled(This,Control,coord,axis,pVal)	\
    (This)->lpVtbl -> GetAxisEnabled(This,Control,coord,axis,pVal)

#define IPmacNC_GetProgramName(This,Control,coord,bMain,pVal)	\
    (This)->lpVtbl -> GetProgramName(This,Control,coord,bMain,pVal)

#define IPmacNC_GetProgramPath(This,Control,coord,bMain,pVal)	\
    (This)->lpVtbl -> GetProgramPath(This,Control,coord,bMain,pVal)

#define IPmacNC_get_Semaphore(This,Control,coord,pVal)	\
    (This)->lpVtbl -> get_Semaphore(This,Control,coord,pVal)

#define IPmacNC_put_Semaphore(This,Control,coord,newVal)	\
    (This)->lpVtbl -> put_Semaphore(This,Control,coord,newVal)

#define IPmacNC_GetProgramStatus(This,Control,coord,pProgID,pNumLines,pLine,pParseLine,pRepeat,pCount,pbSuccess)	\
    (This)->lpVtbl -> GetProgramStatus(This,Control,coord,pProgID,pNumLines,pLine,pParseLine,pRepeat,pCount,pbSuccess)

#define IPmacNC_GetProgramNumber(This,Control,coord,pVal)	\
    (This)->lpVtbl -> GetProgramNumber(This,Control,coord,pVal)

#define IPmacNC_GetCurrentLabel(This,Control,coord,pVal)	\
    (This)->lpVtbl -> GetCurrentLabel(This,Control,coord,pVal)

#define IPmacNC_get_DistanceSelect(This,Control,pVal)	\
    (This)->lpVtbl -> get_DistanceSelect(This,Control,pVal)

#define IPmacNC_put_DistanceSelect(This,Control,newVal)	\
    (This)->lpVtbl -> put_DistanceSelect(This,Control,newVal)

#define IPmacNC_GetJogStep(This,Control,coord,pVal)	\
    (This)->lpVtbl -> GetJogStep(This,Control,coord,pVal)

#define IPmacNC_GetHandleStep(This,Control,coord,pVal)	\
    (This)->lpVtbl -> GetHandleStep(This,Control,coord,pVal)

#define IPmacNC_SetOrigin(This,Control,coord,bOrginAll,adr,bClear,val,metric)	\
    (This)->lpVtbl -> SetOrigin(This,Control,coord,bOrginAll,adr,bClear,val,metric)

#define IPmacNC_ZeroShift(This,Control,coord,bShiftAll,adr,bClear,val,bMetric)	\
    (This)->lpVtbl -> ZeroShift(This,Control,coord,bShiftAll,adr,bClear,val,bMetric)

#define IPmacNC_GetVectorVelocity(This,Control,coord,pVal)	\
    (This)->lpVtbl -> GetVectorVelocity(This,Control,coord,pVal)

#define IPmacNC_ReadMdiBuffer(This,Control,coord,hDlg,idControl)	\
    (This)->lpVtbl -> ReadMdiBuffer(This,Control,coord,hDlg,idControl)

#define IPmacNC_LoadMdiBuffer(This,Control,coord,hDlg,idControl,nRepeat)	\
    (This)->lpVtbl -> LoadMdiBuffer(This,Control,coord,hDlg,idControl,nRepeat)

#define IPmacNC_ClearTextBuffer(This,Control,coord)	\
    (This)->lpVtbl -> ClearTextBuffer(This,Control,coord)

#define IPmacNC_GetCurrentGValue(This,Control,coord,group,pVal)	\
    (This)->lpVtbl -> GetCurrentGValue(This,Control,coord,group,pVal)

#define IPmacNC_get_AxisSelect(This,Control,coord,pVal)	\
    (This)->lpVtbl -> get_AxisSelect(This,Control,coord,pVal)

#define IPmacNC_put_AxisSelect(This,Control,coord,newVal)	\
    (This)->lpVtbl -> put_AxisSelect(This,Control,coord,newVal)

#define IPmacNC_GetInposition(This,Control,coord,pVal)	\
    (This)->lpVtbl -> GetInposition(This,Control,coord,pVal)

#define IPmacNC_get_BufferMode(This,Control,coord,pVal)	\
    (This)->lpVtbl -> get_BufferMode(This,Control,coord,pVal)

#define IPmacNC_put_BufferMode(This,Control,coord,newVal)	\
    (This)->lpVtbl -> put_BufferMode(This,Control,coord,newVal)

#define IPmacNC_GetProgramMode(This,Control,coord,pVal)	\
    (This)->lpVtbl -> GetProgramMode(This,Control,coord,pVal)

#define IPmacNC_GetSelectedBufferMode(This,Control,pVal)	\
    (This)->lpVtbl -> GetSelectedBufferMode(This,Control,pVal)

#define IPmacNC_GetSelectedAxisMotionMode(This,Control,pVal)	\
    (This)->lpVtbl -> GetSelectedAxisMotionMode(This,Control,pVal)

#define IPmacNC_GetProgramMotionMode(This,Control,coord,pVal)	\
    (This)->lpVtbl -> GetProgramMotionMode(This,Control,coord,pVal)

#define IPmacNC_GetBufferOpen(This,Control,pVal)	\
    (This)->lpVtbl -> GetBufferOpen(This,Control,pVal)

#define IPmacNC_GetBufferRemaining(This,Control,coord,pRemain,pParsed)	\
    (This)->lpVtbl -> GetBufferRemaining(This,Control,coord,pRemain,pParsed)

#define IPmacNC_get_SpindleSelect(This,Control,pVal)	\
    (This)->lpVtbl -> get_SpindleSelect(This,Control,pVal)

#define IPmacNC_put_SpindleSelect(This,Control,newVal)	\
    (This)->lpVtbl -> put_SpindleSelect(This,Control,newVal)

#define IPmacNC_get_SpindleRPM(This,Control,pVal)	\
    (This)->lpVtbl -> get_SpindleRPM(This,Control,pVal)

#define IPmacNC_put_SpindleRPM(This,Control,newVal)	\
    (This)->lpVtbl -> put_SpindleRPM(This,Control,newVal)

#define IPmacNC_get_SpindleCSS(This,Control,pVal)	\
    (This)->lpVtbl -> get_SpindleCSS(This,Control,pVal)

#define IPmacNC_put_SpindleCSS(This,Control,newVal)	\
    (This)->lpVtbl -> put_SpindleCSS(This,Control,newVal)

#define IPmacNC_get_SpindleCSSMode(This,Control,pVal)	\
    (This)->lpVtbl -> get_SpindleCSSMode(This,Control,pVal)

#define IPmacNC_put_SpindleCSSMode(This,Control,newVal)	\
    (This)->lpVtbl -> put_SpindleCSSMode(This,Control,newVal)

#define IPmacNC_get_SpindleCSSUnits(This,Control,pVal)	\
    (This)->lpVtbl -> get_SpindleCSSUnits(This,Control,pVal)

#define IPmacNC_put_SpindleCSSUnits(This,Control,newVal)	\
    (This)->lpVtbl -> put_SpindleCSSUnits(This,Control,newVal)

#define IPmacNC_GetSpindleActRPM(This,Control,pVal)	\
    (This)->lpVtbl -> GetSpindleActRPM(This,Control,pVal)

#define IPmacNC_get_SpindleMaxRPM(This,Control,pVal)	\
    (This)->lpVtbl -> get_SpindleMaxRPM(This,Control,pVal)

#define IPmacNC_put_SpindleMaxRPM(This,Control,newVal)	\
    (This)->lpVtbl -> put_SpindleMaxRPM(This,Control,newVal)

#define IPmacNC_get_SpindleOverride(This,Control,coord,pVal)	\
    (This)->lpVtbl -> get_SpindleOverride(This,Control,coord,pVal)

#define IPmacNC_put_SpindleOverride(This,Control,coord,newVal)	\
    (This)->lpVtbl -> put_SpindleOverride(This,Control,coord,newVal)

#define IPmacNC_get_FeedrateOverride(This,Control,coord,pVal)	\
    (This)->lpVtbl -> get_FeedrateOverride(This,Control,coord,pVal)

#define IPmacNC_put_FeedrateOverride(This,Control,coord,newVal)	\
    (This)->lpVtbl -> put_FeedrateOverride(This,Control,coord,newVal)

#define IPmacNC_get_RapidOverride(This,Control,coord,pVal)	\
    (This)->lpVtbl -> get_RapidOverride(This,Control,coord,pVal)

#define IPmacNC_put_RapidOverride(This,Control,coord,newVal)	\
    (This)->lpVtbl -> put_RapidOverride(This,Control,coord,newVal)

#define IPmacNC_get_CoolantSelect(This,Control,pVal)	\
    (This)->lpVtbl -> get_CoolantSelect(This,Control,pVal)

#define IPmacNC_put_CoolantSelect(This,Control,newVal)	\
    (This)->lpVtbl -> put_CoolantSelect(This,Control,newVal)

#define IPmacNC_get_TimebaseMode(This,Control,coord,pVal)	\
    (This)->lpVtbl -> get_TimebaseMode(This,Control,coord,pVal)

#define IPmacNC_put_TimebaseMode(This,Control,coord,newVal)	\
    (This)->lpVtbl -> put_TimebaseMode(This,Control,coord,newVal)

#define IPmacNC_GetFeedrate(This,Control,coord,pMode,pVal)	\
    (This)->lpVtbl -> GetFeedrate(This,Control,coord,pMode,pVal)

#define IPmacNC_SetFeedrate(This,Control,coord,Mode,newVal)	\
    (This)->lpVtbl -> SetFeedrate(This,Control,coord,Mode,newVal)

#define IPmacNC_get_ThreadLead(This,Control,coord,pVal)	\
    (This)->lpVtbl -> get_ThreadLead(This,Control,coord,pVal)

#define IPmacNC_put_ThreadLead(This,Control,coord,newVal)	\
    (This)->lpVtbl -> put_ThreadLead(This,Control,coord,newVal)

#define IPmacNC_GetNumOfTools(This,Control,coord,pVal)	\
    (This)->lpVtbl -> GetNumOfTools(This,Control,coord,pVal)

#define IPmacNC_get_CurrentToolNumber(This,Control,coord,pVal)	\
    (This)->lpVtbl -> get_CurrentToolNumber(This,Control,coord,pVal)

#define IPmacNC_put_CurrentToolNumber(This,Control,coord,newVal)	\
    (This)->lpVtbl -> put_CurrentToolNumber(This,Control,coord,newVal)

#define IPmacNC_get_NextToolNumber(This,Control,coord,pVal)	\
    (This)->lpVtbl -> get_NextToolNumber(This,Control,coord,pVal)

#define IPmacNC_put_NextToolNumber(This,Control,coord,newVal)	\
    (This)->lpVtbl -> put_NextToolNumber(This,Control,coord,newVal)

#define IPmacNC_get_CurrentToolHolderNumber(This,Control,coord,pVal)	\
    (This)->lpVtbl -> get_CurrentToolHolderNumber(This,Control,coord,pVal)

#define IPmacNC_put_CurrentToolHolderNumber(This,Control,coord,newVal)	\
    (This)->lpVtbl -> put_CurrentToolHolderNumber(This,Control,coord,newVal)

#define IPmacNC_SetToolHolderNumber(This,Control,coord,tool,holder)	\
    (This)->lpVtbl -> SetToolHolderNumber(This,Control,coord,tool,holder)

#define IPmacNC_GetToolHolderNumber(This,Control,coord,tool,pVal)	\
    (This)->lpVtbl -> GetToolHolderNumber(This,Control,coord,tool,pVal)

#define IPmacNC_ZeroAllToolOffsets(This,Control,coord)	\
    (This)->lpVtbl -> ZeroAllToolOffsets(This,Control,coord)

#define IPmacNC_GetTypeToolOffset(This,Control,coord,tool,chAxis,type,bMetric,pVal)	\
    (This)->lpVtbl -> GetTypeToolOffset(This,Control,coord,tool,chAxis,type,bMetric,pVal)

#define IPmacNC_SetTypeToolOffset(This,Control,coord,tool,chAxis,type,bMetric,bConfirm,newVal,pbSuccess)	\
    (This)->lpVtbl -> SetTypeToolOffset(This,Control,coord,tool,chAxis,type,bMetric,bConfirm,newVal,pbSuccess)

#define IPmacNC_get_ToolInSpindle(This,Control,coord,pVal)	\
    (This)->lpVtbl -> get_ToolInSpindle(This,Control,coord,pVal)

#define IPmacNC_put_ToolInSpindle(This,Control,coord,newVal)	\
    (This)->lpVtbl -> put_ToolInSpindle(This,Control,coord,newVal)

#define IPmacNC_get_ToolDirection(This,Control,coord,tool,pVal)	\
    (This)->lpVtbl -> get_ToolDirection(This,Control,coord,tool,pVal)

#define IPmacNC_put_ToolDirection(This,Control,coord,tool,newVal)	\
    (This)->lpVtbl -> put_ToolDirection(This,Control,coord,tool,newVal)

#define IPmacNC_get_SpindleCountsPerRev(This,Control,pVal)	\
    (This)->lpVtbl -> get_SpindleCountsPerRev(This,Control,pVal)

#define IPmacNC_put_SpindleCountsPerRev(This,Control,newVal)	\
    (This)->lpVtbl -> put_SpindleCountsPerRev(This,Control,newVal)

#define IPmacNC_get_SpindleGearRatio(This,Control,pVal)	\
    (This)->lpVtbl -> get_SpindleGearRatio(This,Control,pVal)

#define IPmacNC_put_SpindleGearRatio(This,Control,newVal)	\
    (This)->lpVtbl -> put_SpindleGearRatio(This,Control,newVal)

#define IPmacNC_get_SpindleDetect(This,Control,pVal)	\
    (This)->lpVtbl -> get_SpindleDetect(This,Control,pVal)

#define IPmacNC_put_SpindleDetect(This,Control,newVal)	\
    (This)->lpVtbl -> put_SpindleDetect(This,Control,newVal)

#define IPmacNC_get_SpindleAtSpeed(This,Control,pVal)	\
    (This)->lpVtbl -> get_SpindleAtSpeed(This,Control,pVal)

#define IPmacNC_put_SpindleAtSpeed(This,Control,newVal)	\
    (This)->lpVtbl -> put_SpindleAtSpeed(This,Control,newVal)

#define IPmacNC_get_SpindleAtZero(This,Control,pVal)	\
    (This)->lpVtbl -> get_SpindleAtZero(This,Control,pVal)

#define IPmacNC_put_SpindleAtZero(This,Control,newVal)	\
    (This)->lpVtbl -> put_SpindleAtZero(This,Control,newVal)

#define IPmacNC_get_SpindleFPR(This,Control,pVal)	\
    (This)->lpVtbl -> get_SpindleFPR(This,Control,pVal)

#define IPmacNC_put_SpindleFPR(This,Control,newVal)	\
    (This)->lpVtbl -> put_SpindleFPR(This,Control,newVal)

#define IPmacNC_get_HomeInit(This,Control,pVal)	\
    (This)->lpVtbl -> get_HomeInit(This,Control,pVal)

#define IPmacNC_put_HomeInit(This,Control,newVal)	\
    (This)->lpVtbl -> put_HomeInit(This,Control,newVal)

#define IPmacNC_get_HomeMotorMask(This,Control,pVal)	\
    (This)->lpVtbl -> get_HomeMotorMask(This,Control,pVal)

#define IPmacNC_put_HomeMotorMask(This,Control,newVal)	\
    (This)->lpVtbl -> put_HomeMotorMask(This,Control,newVal)

#define IPmacNC_GetActiveGCode(This,Control,coord,group,pVal)	\
    (This)->lpVtbl -> GetActiveGCode(This,Control,coord,group,pVal)

#define IPmacNC_GetActiveGCodeStr(This,Control,coord,group,pVal)	\
    (This)->lpVtbl -> GetActiveGCodeStr(This,Control,coord,group,pVal)

#define IPmacNC_GetToolOffset(This,Control,coord,pVal)	\
    (This)->lpVtbl -> GetToolOffset(This,Control,coord,pVal)

#define IPmacNC_GetCompOffset(This,Control,coord,pVal)	\
    (This)->lpVtbl -> GetCompOffset(This,Control,coord,pVal)

#define IPmacNC_UpdateAllMotors(This,Control)	\
    (This)->lpVtbl -> UpdateAllMotors(This,Control)

#define IPmacNC_GetOperatingTime(This,Control,pDays,pHours,pMinutes,pSeconds)	\
    (This)->lpVtbl -> GetOperatingTime(This,Control,pDays,pHours,pMinutes,pSeconds)

#define IPmacNC_GetTotalCuttingTime(This,Control,pDays,pHours,pMinutes,pSeconds)	\
    (This)->lpVtbl -> GetTotalCuttingTime(This,Control,pDays,pHours,pMinutes,pSeconds)

#define IPmacNC_GetRunningTime(This,Control,pDays,pHours,pMinutes,pSeconds)	\
    (This)->lpVtbl -> GetRunningTime(This,Control,pDays,pHours,pMinutes,pSeconds)

#define IPmacNC_GetPartTime(This,Control,pHours,pMinutes,pSeconds)	\
    (This)->lpVtbl -> GetPartTime(This,Control,pHours,pMinutes,pSeconds)

#define IPmacNC_GetCycleCuttingTime(This,Control,pHours,pMinutes,pSeconds)	\
    (This)->lpVtbl -> GetCycleCuttingTime(This,Control,pHours,pMinutes,pSeconds)

#define IPmacNC_GetCycleTime(This,Control,pHours,pMinutes,Seconds)	\
    (This)->lpVtbl -> GetCycleTime(This,Control,pHours,pMinutes,Seconds)

#define IPmacNC_get_PartsRequired(This,Control,pVal)	\
    (This)->lpVtbl -> get_PartsRequired(This,Control,pVal)

#define IPmacNC_put_PartsRequired(This,Control,newVal)	\
    (This)->lpVtbl -> put_PartsRequired(This,Control,newVal)

#define IPmacNC_get_PartsCount(This,Control,pVal)	\
    (This)->lpVtbl -> get_PartsCount(This,Control,pVal)

#define IPmacNC_put_PartsCount(This,Control,newVal)	\
    (This)->lpVtbl -> put_PartsCount(This,Control,newVal)

#define IPmacNC_get_PartsTotal(This,Control,pVal)	\
    (This)->lpVtbl -> get_PartsTotal(This,Control,pVal)

#define IPmacNC_put_PartsTotal(This,Control,newVal)	\
    (This)->lpVtbl -> put_PartsTotal(This,Control,newVal)

#define IPmacNC_GetNumDisplayedAxis(This,Control,coord,pVal)	\
    (This)->lpVtbl -> GetNumDisplayedAxis(This,Control,coord,pVal)

#define IPmacNC_GetNumDisplayAxisTotal(This,Control,pVal)	\
    (This)->lpVtbl -> GetNumDisplayAxisTotal(This,Control,pVal)

#define IPmacNC_GetMotorPosStr(This,Control,coord,motor,t,bCombined,pAdr,pPos,pbSuccess)	\
    (This)->lpVtbl -> GetMotorPosStr(This,Control,coord,motor,t,bCombined,pAdr,pPos,pbSuccess)

#define IPmacNC_get_IsSpindle(This,Control,coord,motor,pVal)	\
    (This)->lpVtbl -> get_IsSpindle(This,Control,coord,motor,pVal)

#define IPmacNC_put_IsSpindle(This,Control,coord,motor,newVal)	\
    (This)->lpVtbl -> put_IsSpindle(This,Control,coord,motor,newVal)

#define IPmacNC_get_IsDisplayed(This,Control,coord,motor,pVal)	\
    (This)->lpVtbl -> get_IsDisplayed(This,Control,coord,motor,pVal)

#define IPmacNC_put_IsDisplayed(This,Control,coord,motor,newVal)	\
    (This)->lpVtbl -> put_IsDisplayed(This,Control,coord,motor,newVal)

#define IPmacNC_GetMotorUnitsStr(This,Control,coord,motor,pVal,pbSuccess)	\
    (This)->lpVtbl -> GetMotorUnitsStr(This,Control,coord,motor,pVal,pbSuccess)

#define IPmacNC_GetProgramLines(This,Control,coord,start,count,pVal,pbSuccess)	\
    (This)->lpVtbl -> GetProgramLines(This,Control,coord,start,count,pVal,pbSuccess)

#define IPmacNC_GetProgramLine(This,Control,coord,lineNumber,pVal,pbSuccess)	\
    (This)->lpVtbl -> GetProgramLine(This,Control,coord,lineNumber,pVal,pbSuccess)

#define IPmacNC_GetInputLong(This,Control,num,pVal)	\
    (This)->lpVtbl -> GetInputLong(This,Control,num,pVal)

#define IPmacNC_GetOutputLong(This,Control,num,pVal)	\
    (This)->lpVtbl -> GetOutputLong(This,Control,num,pVal)

#define IPmacNC_GetCommandLong(This,Control,num,pVal)	\
    (This)->lpVtbl -> GetCommandLong(This,Control,num,pVal)

#define IPmacNC_GetStatusLong(This,Control,num,pVal)	\
    (This)->lpVtbl -> GetStatusLong(This,Control,num,pVal)

#define IPmacNC_get_HomeInProgress(This,Control,pVal)	\
    (This)->lpVtbl -> get_HomeInProgress(This,Control,pVal)

#define IPmacNC_get_SpindleCmdRPM(This,Control,pVal)	\
    (This)->lpVtbl -> get_SpindleCmdRPM(This,Control,pVal)

#define IPmacNC_Command(This,Control,id,iv,fv,bstr,pbSuccess)	\
    (This)->lpVtbl -> Command(This,Control,id,iv,fv,bstr,pbSuccess)

#define IPmacNC_AddErrorRecord(This,Control,errnum,em,et,line,ax,msg)	\
    (This)->lpVtbl -> AddErrorRecord(This,Control,errnum,em,et,line,ax,msg)

#define IPmacNC_GetParametricDouble(This,Control,coord,region,index,pVal,pbSuccess)	\
    (This)->lpVtbl -> GetParametricDouble(This,Control,coord,region,index,pVal,pbSuccess)

#define IPmacNC_SetParametricDouble(This,Control,coord,region,index,newVal,pbSuccess)	\
    (This)->lpVtbl -> SetParametricDouble(This,Control,coord,region,index,newVal,pbSuccess)

#define IPmacNC_GetSelectedAxisChar(This,Control,pAxChar)	\
    (This)->lpVtbl -> GetSelectedAxisChar(This,Control,pAxChar)

#define IPmacNC_get_CurrentErrorLevel(This,Control,pVal)	\
    (This)->lpVtbl -> get_CurrentErrorLevel(This,Control,pVal)

#define IPmacNC_put_CurrentErrorLevel(This,Control,newVal)	\
    (This)->lpVtbl -> put_CurrentErrorLevel(This,Control,newVal)

#define IPmacNC_GetAxisFormat(This,Control,coord,ax,bMetric,pLen,pDecPlaces)	\
    (This)->lpVtbl -> GetAxisFormat(This,Control,coord,ax,bMetric,pLen,pDecPlaces)

#define IPmacNC_GetAxisFormatStr(This,Control,coord,ax,bMetric,pVal)	\
    (This)->lpVtbl -> GetAxisFormatStr(This,Control,coord,ax,bMetric,pVal)

#define IPmacNC_SetAxisFormat(This,Control,coord,ax,bMetric,len,decPlaces)	\
    (This)->lpVtbl -> SetAxisFormat(This,Control,coord,ax,bMetric,len,decPlaces)

#define IPmacNC_GetFirstError(This,Control,pHeader,pMessage,pCode,pFilename,pCount,pChanged)	\
    (This)->lpVtbl -> GetFirstError(This,Control,pHeader,pMessage,pCode,pFilename,pCount,pChanged)

#define IPmacNC_GetNextError(This,Control,pHeader,pMessage,pCode,pFilename,pbMore)	\
    (This)->lpVtbl -> GetNextError(This,Control,pHeader,pMessage,pCode,pFilename,pbMore)

#define IPmacNC_OffsetsUpdateClear(This,Control,clearMask)	\
    (This)->lpVtbl -> OffsetsUpdateClear(This,Control,clearMask)

#define IPmacNC_OffsetsUpdate(This,Control,pVal)	\
    (This)->lpVtbl -> OffsetsUpdate(This,Control,pVal)

#define IPmacNC_ReadErrorMsgBoxText(This,Control,index,pCount,pText,pTitle)	\
    (This)->lpVtbl -> ReadErrorMsgBoxText(This,Control,index,pCount,pText,pTitle)

#define IPmacNC_ResetMsgBoxError(This,Control,errRetVal)	\
    (This)->lpVtbl -> ResetMsgBoxError(This,Control,errRetVal)

#define IPmacNC_SaveParametricVars(This,Control)	\
    (This)->lpVtbl -> SaveParametricVars(This,Control)

#define IPmacNC_LoadParametricVars(This,Control)	\
    (This)->lpVtbl -> LoadParametricVars(This,Control)

#define IPmacNC_ResetCycleTime(This,Control)	\
    (This)->lpVtbl -> ResetCycleTime(This,Control)

#define IPmacNC_GetToolGuageOffset(This,Control,coord,tool,bMulti,ax,bMetric,pVal)	\
    (This)->lpVtbl -> GetToolGuageOffset(This,Control,coord,tool,bMulti,ax,bMetric,pVal)

#define IPmacNC_SetToolGuageOffset(This,Control,coord,tool,bMulti,ax,bMetric,newVal)	\
    (This)->lpVtbl -> SetToolGuageOffset(This,Control,coord,tool,bMulti,ax,bMetric,newVal)

#define IPmacNC_AutoSetToolOffset(This,Control,coord,tool,axis)	\
    (This)->lpVtbl -> AutoSetToolOffset(This,Control,coord,tool,axis)

#define IPmacNC_GetOffsetValue(This,Control,coord,axis,t,bMetric,pVal)	\
    (This)->lpVtbl -> GetOffsetValue(This,Control,coord,axis,t,bMetric,pVal)

#define IPmacNC_SetOffsetValue(This,Control,coord,axis,t,bMetric,bConfirm,NewVal,pbSuccess)	\
    (This)->lpVtbl -> SetOffsetValue(This,Control,coord,axis,t,bMetric,bConfirm,NewVal,pbSuccess)

#define IPmacNC_AutoSetWorkOffset(This,Control,coord,axis,t,pbSuccess)	\
    (This)->lpVtbl -> AutoSetWorkOffset(This,Control,coord,axis,t,pbSuccess)

#define IPmacNC_SearchText(This,Control,coord,text,bMatchCase,pbSuccess)	\
    (This)->lpVtbl -> SearchText(This,Control,coord,text,bMatchCase,pbSuccess)

#define IPmacNC_SearchTextNext(This,Control,coord,text,bMatchCase,pbSuccess)	\
    (This)->lpVtbl -> SearchTextNext(This,Control,coord,text,bMatchCase,pbSuccess)

#define IPmacNC_GetBufferText(This,Control,coord,bMDI,pVal)	\
    (This)->lpVtbl -> GetBufferText(This,Control,coord,bMDI,pVal)

#define IPmacNC_SetBufferText(This,Control,coord,repeat,bMDI,newVal)	\
    (This)->lpVtbl -> SetBufferText(This,Control,coord,repeat,bMDI,newVal)

#define IPmacNC_GetProcessorClocks(This,pIO,pLA,pParse,pFormat,pWrite)	\
    (This)->lpVtbl -> GetProcessorClocks(This,pIO,pLA,pParse,pFormat,pWrite)

#define IPmacNC_get_CurrentErrorCount(This,Control,pVal)	\
    (This)->lpVtbl -> get_CurrentErrorCount(This,Control,pVal)

#define IPmacNC_get_BlocksPerSecond(This,Control,Coord,pVal)	\
    (This)->lpVtbl -> get_BlocksPerSecond(This,Control,Coord,pVal)

#define IPmacNC_GetMetricInput(This,Control,coord,pVal)	\
    (This)->lpVtbl -> GetMetricInput(This,Control,coord,pVal)

#define IPmacNC_GetToolInfo(This,Control,coord,pCurrentTool,pNextTool,pHolder,pHolderTool)	\
    (This)->lpVtbl -> GetToolInfo(This,Control,coord,pCurrentTool,pNextTool,pHolder,pHolderTool)

#define IPmacNC_GetToolData(This,Control,Coord,Tool,pVal)	\
    (This)->lpVtbl -> GetToolData(This,Control,Coord,Tool,pVal)

#define IPmacNC_ZeroAllTypeToolOffsets(This,Control,coord,type)	\
    (This)->lpVtbl -> ZeroAllTypeToolOffsets(This,Control,coord,type)

#define IPmacNC_SearchLine(This,Control,Coord,Line,pbSuccess)	\
    (This)->lpVtbl -> SearchLine(This,Control,Coord,Line,pbSuccess)

#define IPmacNC_GetAxisMotor(This,Control,Coord,Axis,pMotor,pbSuccess)	\
    (This)->lpVtbl -> GetAxisMotor(This,Control,Coord,Axis,pMotor,pbSuccess)

#define IPmacNC_GetMotorAxis(This,Control,Coord,Motor,pAxis,pbSuccess)	\
    (This)->lpVtbl -> GetMotorAxis(This,Control,Coord,Motor,pAxis,pbSuccess)

#define IPmacNC_GetMotorPos(This,Control,coord,motor,t,pAdr,pPos,pbSuccess)	\
    (This)->lpVtbl -> GetMotorPos(This,Control,coord,motor,t,pAdr,pPos,pbSuccess)

#define IPmacNC_GetAllAxisPos(This,Control,coord,t,pvAdrArray,pvPosArray,pbSuccess)	\
    (This)->lpVtbl -> GetAllAxisPos(This,Control,coord,t,pvAdrArray,pvPosArray,pbSuccess)

#define IPmacNC_GetAllAxisPosStr(This,Control,coord,t,bCoord,pvArray,pbSuccess)	\
    (This)->lpVtbl -> GetAllAxisPosStr(This,Control,coord,t,bCoord,pvArray,pbSuccess)

#define IPmacNC_GetMaxFeedrates(This,Control,coord,pMaxPerRev,pMaxPerMinute)	\
    (This)->lpVtbl -> GetMaxFeedrates(This,Control,coord,pMaxPerRev,pMaxPerMinute)

#define IPmacNC_SetMaxFeedrates(This,Control,coord,MaxPerRev,MaxPerMinute)	\
    (This)->lpVtbl -> SetMaxFeedrates(This,Control,coord,MaxPerRev,MaxPerMinute)

#define IPmacNC_ClearOffsetValue(This,Control,coord,axis,t,bMetric,NewVal,pbSuccess)	\
    (This)->lpVtbl -> ClearOffsetValue(This,Control,coord,axis,t,bMetric,NewVal,pbSuccess)

#define IPmacNC_GetToolDataStruct(This,Control,Coord,Tool,pStruct)	\
    (This)->lpVtbl -> GetToolDataStruct(This,Control,Coord,Tool,pStruct)

#define IPmacNC_StartDataCollection(This,Control,CollectionMSec,BufferSize,pbSuccess)	\
    (This)->lpVtbl -> StartDataCollection(This,Control,CollectionMSec,BufferSize,pbSuccess)

#define IPmacNC_StopDataCollection(This,Control)	\
    (This)->lpVtbl -> StopDataCollection(This,Control)

#define IPmacNC_DataCollect(This,Control,Coord,pvArray,pbSuccess)	\
    (This)->lpVtbl -> DataCollect(This,Control,Coord,pvArray,pbSuccess)

#define IPmacNC_ResetPartsCount(This,Control)	\
    (This)->lpVtbl -> ResetPartsCount(This,Control)

#define IPmacNC_ResetPartsTotal(This,Control)	\
    (This)->lpVtbl -> ResetPartsTotal(This,Control)

#define IPmacNC_GetWorkOffset(This,Control,coord,t,pExtendedVal)	\
    (This)->lpVtbl -> GetWorkOffset(This,Control,coord,t,pExtendedVal)

#define IPmacNC_get_DataCollectionEnabled(This,Control,pVal)	\
    (This)->lpVtbl -> get_DataCollectionEnabled(This,Control,pVal)

#define IPmacNC_get_DataCollectionResolution(This,Control,pVal)	\
    (This)->lpVtbl -> get_DataCollectionResolution(This,Control,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_Open_Proxy( 
    IPmacNC * This,
    /* [in] */ long Device,
    /* [in] */ long Control,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_Open_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_Close_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control);


void __RPC_STUB IPmacNC_Close_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_OpenFile_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long dwCoord,
    /* [in] */ BSTR szFilePath,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_OpenFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_CloseFile_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long dwCoord,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_CloseFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_Title_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IPmacNC_get_Title_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_Title_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ BSTR newVal);


void __RPC_STUB IPmacNC_put_Title_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_DeviceNumber_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacNC_get_DeviceNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_IsOpen_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacNC_get_IsOpen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_SetMachineMode_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ NCMODE mode);


void __RPC_STUB IPmacNC_SetMachineMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetMachineMode_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [out] */ NCMODE *pMode);


void __RPC_STUB IPmacNC_GetMachineMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_Mode_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ NCMODE *pVal);


void __RPC_STUB IPmacNC_get_Mode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_Mode_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ NCMODE newVal);


void __RPC_STUB IPmacNC_put_Mode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetRegistryBool_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ BSTR section,
    /* [in] */ BSTR name,
    /* [in] */ VARIANT_BOOL def,
    /* [out] */ VARIANT_BOOL *pVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_GetRegistryBool_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_SetRegistryBool_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ BSTR section,
    /* [in] */ BSTR name,
    /* [in] */ VARIANT_BOOL bVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_SetRegistryBool_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetRegistryLong_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ BSTR section,
    /* [in] */ BSTR name,
    /* [in] */ long def,
    /* [out] */ long *pVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_GetRegistryLong_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_SetRegistryLong_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ BSTR section,
    /* [in] */ BSTR name,
    /* [in] */ long lVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_SetRegistryLong_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetRegistryString_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ BSTR section,
    /* [in] */ BSTR name,
    /* [in] */ BSTR def,
    /* [out][in] */ BSTR *pVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_GetRegistryString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_SetRegistryString_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ BSTR section,
    /* [in] */ BSTR name,
    /* [in] */ BSTR Val,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_SetRegistryString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetRegistryDouble_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ BSTR section,
    /* [in] */ BSTR name,
    /* [in] */ double def,
    /* [out] */ double *pVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_GetRegistryDouble_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_SetRegistryDouble_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ BSTR section,
    /* [in] */ BSTR name,
    /* [in] */ double Val,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_SetRegistryDouble_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_CoordSystem_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacNC_get_CoordSystem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_CoordSystem_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long newVal);


void __RPC_STUB IPmacNC_put_CoordSystem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_NumCoordSystems_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacNC_get_NumCoordSystems_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_SingleBlock_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacNC_get_SingleBlock_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_SingleBlock_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IPmacNC_put_SingleBlock_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_BlockDelete_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacNC_get_BlockDelete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_BlockDelete_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IPmacNC_put_BlockDelete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_OptionalStop_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacNC_get_OptionalStop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_OptionalStop_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IPmacNC_put_OptionalStop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_MachineLock_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacNC_get_MachineLock_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_MachineLock_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IPmacNC_put_MachineLock_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_DryRun_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacNC_get_DryRun_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_DryRun_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IPmacNC_put_DryRun_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_MachineType_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ NCMACHINETYPE *pVal);


void __RPC_STUB IPmacNC_get_MachineType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_JogSelect_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ NCJOGSELECT *pVal);


void __RPC_STUB IPmacNC_get_JogSelect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_JogSelect_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ NCJOGSELECT newVal);


void __RPC_STUB IPmacNC_put_JogSelect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_SpeedSelect_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ NCSPEEDSELECT *pVal);


void __RPC_STUB IPmacNC_get_SpeedSelect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_SpeedSelect_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ NCSPEEDSELECT newVal);


void __RPC_STUB IPmacNC_put_SpeedSelect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_ProgramLoaded_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacNC_get_ProgramLoaded_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetAxisEnabled_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ short axis,
    /* [out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacNC_GetAxisEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetProgramName_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ VARIANT_BOOL bMain,
    /* [out][in] */ BSTR *pVal);


void __RPC_STUB IPmacNC_GetProgramName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetProgramPath_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ VARIANT_BOOL bMain,
    /* [out][in] */ BSTR *pVal);


void __RPC_STUB IPmacNC_GetProgramPath_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_Semaphore_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacNC_get_Semaphore_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_Semaphore_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long newVal);


void __RPC_STUB IPmacNC_put_Semaphore_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetProgramStatus_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [out] */ long *pProgID,
    /* [out] */ long *pNumLines,
    /* [out] */ long *pLine,
    /* [out] */ long *pParseLine,
    /* [out] */ long *pRepeat,
    /* [out] */ long *pCount,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_GetProgramStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetProgramNumber_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [out] */ long *pVal);


void __RPC_STUB IPmacNC_GetProgramNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetCurrentLabel_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [out] */ long *pVal);


void __RPC_STUB IPmacNC_GetCurrentLabel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_DistanceSelect_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ NCSPEEDSELECT *pVal);


void __RPC_STUB IPmacNC_get_DistanceSelect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_DistanceSelect_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ NCSPEEDSELECT newVal);


void __RPC_STUB IPmacNC_put_DistanceSelect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetJogStep_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [out] */ double *pVal);


void __RPC_STUB IPmacNC_GetJogStep_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetHandleStep_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [out] */ double *pVal);


void __RPC_STUB IPmacNC_GetHandleStep_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_SetOrigin_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ VARIANT_BOOL bOrginAll,
    /* [in] */ NCADDRESS adr,
    /* [in] */ VARIANT_BOOL bClear,
    /* [in] */ double val,
    /* [in] */ VARIANT_BOOL metric);


void __RPC_STUB IPmacNC_SetOrigin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_ZeroShift_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ VARIANT_BOOL bShiftAll,
    /* [in] */ NCADDRESS adr,
    /* [in] */ VARIANT_BOOL bClear,
    /* [in] */ double val,
    /* [in] */ VARIANT_BOOL bMetric);


void __RPC_STUB IPmacNC_ZeroShift_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetVectorVelocity_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [out] */ double *pVal);


void __RPC_STUB IPmacNC_GetVectorVelocity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_ReadMdiBuffer_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long *hDlg,
    /* [in] */ long idControl);


void __RPC_STUB IPmacNC_ReadMdiBuffer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_LoadMdiBuffer_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long *hDlg,
    /* [in] */ long idControl,
    /* [in] */ long nRepeat);


void __RPC_STUB IPmacNC_LoadMdiBuffer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_ClearTextBuffer_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord);


void __RPC_STUB IPmacNC_ClearTextBuffer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetCurrentGValue_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long group,
    /* [out] */ long *pVal);


void __RPC_STUB IPmacNC_GetCurrentGValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_AxisSelect_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [retval][out] */ NCAXISSELECT *pVal);


void __RPC_STUB IPmacNC_get_AxisSelect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_AxisSelect_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ NCAXISSELECT newVal);


void __RPC_STUB IPmacNC_put_AxisSelect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetInposition_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacNC_GetInposition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_BufferMode_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [retval][out] */ NCBUFFERMODE *pVal);


void __RPC_STUB IPmacNC_get_BufferMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_BufferMode_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ NCBUFFERMODE newVal);


void __RPC_STUB IPmacNC_put_BufferMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetProgramMode_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [out] */ DEVPROGRAMMODE *pVal);


void __RPC_STUB IPmacNC_GetProgramMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetSelectedBufferMode_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [out] */ NCBUFFERMODE *pVal);


void __RPC_STUB IPmacNC_GetSelectedBufferMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetSelectedAxisMotionMode_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [out] */ DEVMOTIONMODE *pVal);


void __RPC_STUB IPmacNC_GetSelectedAxisMotionMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetProgramMotionMode_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [out] */ DEVMOTIONMODE *pVal);


void __RPC_STUB IPmacNC_GetProgramMotionMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetBufferOpen_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacNC_GetBufferOpen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetBufferRemaining_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [out] */ long *pRemain,
    /* [out] */ long *pParsed);


void __RPC_STUB IPmacNC_GetBufferRemaining_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_SpindleSelect_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ NCSPINDLESELECT *pVal);


void __RPC_STUB IPmacNC_get_SpindleSelect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_SpindleSelect_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ NCSPINDLESELECT newVal);


void __RPC_STUB IPmacNC_put_SpindleSelect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_SpindleRPM_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacNC_get_SpindleRPM_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_SpindleRPM_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long newVal);


void __RPC_STUB IPmacNC_put_SpindleRPM_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_SpindleCSS_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ double *pVal);


void __RPC_STUB IPmacNC_get_SpindleCSS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_SpindleCSS_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ double newVal);


void __RPC_STUB IPmacNC_put_SpindleCSS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_SpindleCSSMode_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacNC_get_SpindleCSSMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_SpindleCSSMode_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IPmacNC_put_SpindleCSSMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_SpindleCSSUnits_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ double *pVal);


void __RPC_STUB IPmacNC_get_SpindleCSSUnits_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_SpindleCSSUnits_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ double newVal);


void __RPC_STUB IPmacNC_put_SpindleCSSUnits_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetSpindleActRPM_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [out] */ long *pVal);


void __RPC_STUB IPmacNC_GetSpindleActRPM_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_SpindleMaxRPM_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacNC_get_SpindleMaxRPM_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_SpindleMaxRPM_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long newVal);


void __RPC_STUB IPmacNC_put_SpindleMaxRPM_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_SpindleOverride_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacNC_get_SpindleOverride_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_SpindleOverride_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long newVal);


void __RPC_STUB IPmacNC_put_SpindleOverride_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_FeedrateOverride_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [retval][out] */ double *pVal);


void __RPC_STUB IPmacNC_get_FeedrateOverride_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_FeedrateOverride_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ double newVal);


void __RPC_STUB IPmacNC_put_FeedrateOverride_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_RapidOverride_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [retval][out] */ double *pVal);


void __RPC_STUB IPmacNC_get_RapidOverride_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_RapidOverride_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ double newVal);


void __RPC_STUB IPmacNC_put_RapidOverride_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_CoolantSelect_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ NCCLNTSELECT *pVal);


void __RPC_STUB IPmacNC_get_CoolantSelect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_CoolantSelect_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ NCCLNTSELECT newVal);


void __RPC_STUB IPmacNC_put_CoolantSelect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_TimebaseMode_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [retval][out] */ NCTIMEBASEMODE *pVal);


void __RPC_STUB IPmacNC_get_TimebaseMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_TimebaseMode_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ NCTIMEBASEMODE newVal);


void __RPC_STUB IPmacNC_put_TimebaseMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetFeedrate_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [out] */ NCFEEDRATEMODE *pMode,
    /* [out] */ double *pVal);


void __RPC_STUB IPmacNC_GetFeedrate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_SetFeedrate_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ NCFEEDRATEMODE Mode,
    /* [in] */ double newVal);


void __RPC_STUB IPmacNC_SetFeedrate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_ThreadLead_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [retval][out] */ double *pVal);


void __RPC_STUB IPmacNC_get_ThreadLead_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_ThreadLead_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ double newVal);


void __RPC_STUB IPmacNC_put_ThreadLead_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetNumOfTools_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [out] */ long *pVal);


void __RPC_STUB IPmacNC_GetNumOfTools_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_CurrentToolNumber_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacNC_get_CurrentToolNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_CurrentToolNumber_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long newVal);


void __RPC_STUB IPmacNC_put_CurrentToolNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_NextToolNumber_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacNC_get_NextToolNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_NextToolNumber_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long newVal);


void __RPC_STUB IPmacNC_put_NextToolNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_CurrentToolHolderNumber_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacNC_get_CurrentToolHolderNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_CurrentToolHolderNumber_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long newVal);


void __RPC_STUB IPmacNC_put_CurrentToolHolderNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_SetToolHolderNumber_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long tool,
    /* [in] */ long holder);


void __RPC_STUB IPmacNC_SetToolHolderNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetToolHolderNumber_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long tool,
    /* [out] */ long *pVal);


void __RPC_STUB IPmacNC_GetToolHolderNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_ZeroAllToolOffsets_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord);


void __RPC_STUB IPmacNC_ZeroAllToolOffsets_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetTypeToolOffset_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long tool,
    /* [in] */ BSTR chAxis,
    /* [in] */ NCTOOLOFSTYPE type,
    /* [in] */ VARIANT_BOOL bMetric,
    /* [out] */ double *pVal);


void __RPC_STUB IPmacNC_GetTypeToolOffset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_SetTypeToolOffset_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long tool,
    /* [in] */ BSTR chAxis,
    /* [in] */ NCTOOLOFSTYPE type,
    /* [in] */ VARIANT_BOOL bMetric,
    /* [in] */ VARIANT_BOOL bConfirm,
    /* [in] */ double newVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_SetTypeToolOffset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_ToolInSpindle_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacNC_get_ToolInSpindle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_ToolInSpindle_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long newVal);


void __RPC_STUB IPmacNC_put_ToolInSpindle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_ToolDirection_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long tool,
    /* [retval][out] */ NCTOOLDIRECTION *pVal);


void __RPC_STUB IPmacNC_get_ToolDirection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_ToolDirection_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long tool,
    /* [in] */ NCTOOLDIRECTION newVal);


void __RPC_STUB IPmacNC_put_ToolDirection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_SpindleCountsPerRev_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ double *pVal);


void __RPC_STUB IPmacNC_get_SpindleCountsPerRev_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_SpindleCountsPerRev_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ double newVal);


void __RPC_STUB IPmacNC_put_SpindleCountsPerRev_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_SpindleGearRatio_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ double *pVal);


void __RPC_STUB IPmacNC_get_SpindleGearRatio_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_SpindleGearRatio_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ double newVal);


void __RPC_STUB IPmacNC_put_SpindleGearRatio_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_SpindleDetect_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacNC_get_SpindleDetect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_SpindleDetect_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IPmacNC_put_SpindleDetect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_SpindleAtSpeed_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacNC_get_SpindleAtSpeed_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_SpindleAtSpeed_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IPmacNC_put_SpindleAtSpeed_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_SpindleAtZero_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacNC_get_SpindleAtZero_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_SpindleAtZero_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IPmacNC_put_SpindleAtZero_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_SpindleFPR_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacNC_get_SpindleFPR_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_SpindleFPR_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IPmacNC_put_SpindleFPR_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_HomeInit_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacNC_get_HomeInit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_HomeInit_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IPmacNC_put_HomeInit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_HomeMotorMask_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacNC_get_HomeMotorMask_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_HomeMotorMask_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long newVal);


void __RPC_STUB IPmacNC_put_HomeMotorMask_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetActiveGCode_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long group,
    /* [out] */ double *pVal);


void __RPC_STUB IPmacNC_GetActiveGCode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetActiveGCodeStr_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long group,
    /* [out][in] */ BSTR *pVal);


void __RPC_STUB IPmacNC_GetActiveGCodeStr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetToolOffset_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [out] */ long *pVal);


void __RPC_STUB IPmacNC_GetToolOffset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetCompOffset_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [out] */ long *pVal);


void __RPC_STUB IPmacNC_GetCompOffset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_UpdateAllMotors_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control);


void __RPC_STUB IPmacNC_UpdateAllMotors_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetOperatingTime_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [out] */ long *pDays,
    /* [out] */ long *pHours,
    /* [out] */ long *pMinutes,
    /* [out] */ long *pSeconds);


void __RPC_STUB IPmacNC_GetOperatingTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetTotalCuttingTime_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [out] */ long *pDays,
    /* [out] */ long *pHours,
    /* [out] */ long *pMinutes,
    /* [out] */ long *pSeconds);


void __RPC_STUB IPmacNC_GetTotalCuttingTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetRunningTime_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [out] */ long *pDays,
    /* [out] */ long *pHours,
    /* [out] */ long *pMinutes,
    /* [out] */ long *pSeconds);


void __RPC_STUB IPmacNC_GetRunningTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetPartTime_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [out] */ long *pHours,
    /* [out] */ long *pMinutes,
    /* [out] */ long *pSeconds);


void __RPC_STUB IPmacNC_GetPartTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetCycleCuttingTime_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [out] */ long *pHours,
    /* [out] */ long *pMinutes,
    /* [out] */ long *pSeconds);


void __RPC_STUB IPmacNC_GetCycleCuttingTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetCycleTime_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [out] */ long *pHours,
    /* [out] */ long *pMinutes,
    /* [out] */ long *Seconds);


void __RPC_STUB IPmacNC_GetCycleTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_PartsRequired_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacNC_get_PartsRequired_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_PartsRequired_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long newVal);


void __RPC_STUB IPmacNC_put_PartsRequired_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_PartsCount_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacNC_get_PartsCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_PartsCount_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long newVal);


void __RPC_STUB IPmacNC_put_PartsCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_PartsTotal_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacNC_get_PartsTotal_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_PartsTotal_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long newVal);


void __RPC_STUB IPmacNC_put_PartsTotal_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetNumDisplayedAxis_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [out] */ long *pVal);


void __RPC_STUB IPmacNC_GetNumDisplayedAxis_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetNumDisplayAxisTotal_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [out] */ long *pVal);


void __RPC_STUB IPmacNC_GetNumDisplayAxisTotal_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetMotorPosStr_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long motor,
    /* [in] */ NCPOSTYPE t,
    /* [in] */ VARIANT_BOOL bCombined,
    /* [out][in] */ BSTR *pAdr,
    /* [out][in] */ BSTR *pPos,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_GetMotorPosStr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_IsSpindle_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long motor,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacNC_get_IsSpindle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_IsSpindle_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long motor,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IPmacNC_put_IsSpindle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_IsDisplayed_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long motor,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacNC_get_IsDisplayed_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_IsDisplayed_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long motor,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IPmacNC_put_IsDisplayed_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetMotorUnitsStr_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long motor,
    /* [out][in] */ BSTR *pVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_GetMotorUnitsStr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetProgramLines_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long start,
    /* [in] */ long count,
    /* [out][in] */ VARIANT *pVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_GetProgramLines_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetProgramLine_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long lineNumber,
    /* [out] */ BSTR *pVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_GetProgramLine_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetInputLong_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long num,
    /* [out] */ long *pVal);


void __RPC_STUB IPmacNC_GetInputLong_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetOutputLong_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long num,
    /* [out] */ long *pVal);


void __RPC_STUB IPmacNC_GetOutputLong_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetCommandLong_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long num,
    /* [out] */ long *pVal);


void __RPC_STUB IPmacNC_GetCommandLong_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetStatusLong_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long num,
    /* [out] */ long *pVal);


void __RPC_STUB IPmacNC_GetStatusLong_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_HomeInProgress_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacNC_get_HomeInProgress_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_SpindleCmdRPM_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacNC_get_SpindleCmdRPM_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_Command_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long id,
    /* [in] */ long iv,
    /* [in] */ double fv,
    /* [in] */ BSTR bstr,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_Command_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_AddErrorRecord_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long errnum,
    /* [in] */ NCERRORMODE em,
    /* [in] */ NCERRORTYPE et,
    /* [in] */ long line,
    /* [in] */ short ax,
    /* [in] */ BSTR msg);


void __RPC_STUB IPmacNC_AddErrorRecord_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetParametricDouble_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ BSTR region,
    /* [in] */ long index,
    /* [out] */ double *pVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_GetParametricDouble_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_SetParametricDouble_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ BSTR region,
    /* [in] */ long index,
    /* [in] */ double newVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_SetParametricDouble_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetSelectedAxisChar_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [out] */ short *pAxChar);


void __RPC_STUB IPmacNC_GetSelectedAxisChar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_CurrentErrorLevel_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ NCERRORMODE *pVal);


void __RPC_STUB IPmacNC_get_CurrentErrorLevel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPmacNC_put_CurrentErrorLevel_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ NCERRORMODE newVal);


void __RPC_STUB IPmacNC_put_CurrentErrorLevel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetAxisFormat_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ short ax,
    /* [in] */ VARIANT_BOOL bMetric,
    /* [out] */ long *pLen,
    /* [out] */ long *pDecPlaces);


void __RPC_STUB IPmacNC_GetAxisFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetAxisFormatStr_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ short ax,
    /* [in] */ VARIANT_BOOL bMetric,
    /* [out][in] */ BSTR *pVal);


void __RPC_STUB IPmacNC_GetAxisFormatStr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_SetAxisFormat_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ short ax,
    /* [in] */ VARIANT_BOOL bMetric,
    /* [in] */ long len,
    /* [in] */ long decPlaces);


void __RPC_STUB IPmacNC_SetAxisFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetFirstError_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [out][in] */ BSTR *pHeader,
    /* [out][in] */ BSTR *pMessage,
    /* [out][in] */ BSTR *pCode,
    /* [out][in] */ BSTR *pFilename,
    /* [out] */ long *pCount,
    /* [out] */ long *pChanged);


void __RPC_STUB IPmacNC_GetFirstError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetNextError_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [out][in] */ BSTR *pHeader,
    /* [out][in] */ BSTR *pMessage,
    /* [out][in] */ BSTR *pCode,
    /* [out][in] */ BSTR *pFilename,
    /* [out] */ VARIANT_BOOL *pbMore);


void __RPC_STUB IPmacNC_GetNextError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_OffsetsUpdateClear_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long clearMask);


void __RPC_STUB IPmacNC_OffsetsUpdateClear_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_OffsetsUpdate_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [out] */ long *pVal);


void __RPC_STUB IPmacNC_OffsetsUpdate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_ReadErrorMsgBoxText_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long index,
    /* [out] */ long *pCount,
    /* [out][in] */ BSTR *pText,
    /* [out][in] */ BSTR *pTitle);


void __RPC_STUB IPmacNC_ReadErrorMsgBoxText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_ResetMsgBoxError_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long errRetVal);


void __RPC_STUB IPmacNC_ResetMsgBoxError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_SaveParametricVars_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control);


void __RPC_STUB IPmacNC_SaveParametricVars_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_LoadParametricVars_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control);


void __RPC_STUB IPmacNC_LoadParametricVars_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_ResetCycleTime_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control);


void __RPC_STUB IPmacNC_ResetCycleTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetToolGuageOffset_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long tool,
    /* [in] */ VARIANT_BOOL bMulti,
    /* [in] */ short ax,
    /* [in] */ VARIANT_BOOL bMetric,
    /* [out] */ double *pVal);


void __RPC_STUB IPmacNC_GetToolGuageOffset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_SetToolGuageOffset_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long tool,
    /* [in] */ VARIANT_BOOL bMulti,
    /* [in] */ short ax,
    /* [in] */ VARIANT_BOOL bMetric,
    /* [in] */ double newVal);


void __RPC_STUB IPmacNC_SetToolGuageOffset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_AutoSetToolOffset_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long tool,
    /* [in] */ short axis);


void __RPC_STUB IPmacNC_AutoSetToolOffset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetOffsetValue_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ short axis,
    /* [in] */ NCCOORDTYPE t,
    /* [in] */ VARIANT_BOOL bMetric,
    /* [out] */ double *pVal);


void __RPC_STUB IPmacNC_GetOffsetValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_SetOffsetValue_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ short axis,
    /* [in] */ NCCOORDTYPE t,
    /* [in] */ VARIANT_BOOL bMetric,
    /* [in] */ VARIANT_BOOL bConfirm,
    /* [in] */ double NewVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_SetOffsetValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_AutoSetWorkOffset_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ short axis,
    /* [in] */ NCCOORDTYPE t,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_AutoSetWorkOffset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_SearchText_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ BSTR text,
    /* [in] */ VARIANT_BOOL bMatchCase,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_SearchText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_SearchTextNext_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ BSTR text,
    /* [in] */ VARIANT_BOOL bMatchCase,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_SearchTextNext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetBufferText_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ VARIANT_BOOL bMDI,
    /* [out][in] */ BSTR *pVal);


void __RPC_STUB IPmacNC_GetBufferText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_SetBufferText_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long repeat,
    /* [in] */ VARIANT_BOOL bMDI,
    /* [in] */ BSTR newVal);


void __RPC_STUB IPmacNC_SetBufferText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetProcessorClocks_Proxy( 
    IPmacNC * This,
    /* [out] */ double *pIO,
    /* [out] */ double *pLA,
    /* [out] */ double *pParse,
    /* [out] */ double *pFormat,
    /* [out] */ double *pWrite);


void __RPC_STUB IPmacNC_GetProcessorClocks_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_CurrentErrorCount_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacNC_get_CurrentErrorCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_BlocksPerSecond_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long Coord,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacNC_get_BlocksPerSecond_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetMetricInput_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacNC_GetMetricInput_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetToolInfo_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [out] */ long *pCurrentTool,
    /* [out] */ long *pNextTool,
    /* [out] */ long *pHolder,
    /* [out] */ long *pHolderTool);


void __RPC_STUB IPmacNC_GetToolInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetToolData_Proxy( 
    IPmacNC * This,
    /* [in] */ LONG Control,
    /* [in] */ LONG Coord,
    /* [in] */ LONG Tool,
    /* [out][in] */ VARIANT *pVal);


void __RPC_STUB IPmacNC_GetToolData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_ZeroAllTypeToolOffsets_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ NCTOOLOFSTYPE type);


void __RPC_STUB IPmacNC_ZeroAllTypeToolOffsets_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_SearchLine_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long Coord,
    /* [in] */ long Line,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_SearchLine_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetAxisMotor_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long Coord,
    /* [in] */ short Axis,
    /* [out] */ long *pMotor,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_GetAxisMotor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetMotorAxis_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long Coord,
    /* [in] */ long Motor,
    /* [out] */ short *pAxis,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_GetMotorAxis_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetMotorPos_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ long motor,
    /* [in] */ NCPOSTYPE t,
    /* [out] */ BSTR *pAdr,
    /* [out] */ double *pPos,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_GetMotorPos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetAllAxisPos_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ NCPOSTYPE t,
    /* [out][in] */ SAFEARRAY * *pvAdrArray,
    /* [out][in] */ SAFEARRAY * *pvPosArray,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_GetAllAxisPos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetAllAxisPosStr_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ NCPOSTYPE t,
    /* [in] */ VARIANT_BOOL bCoord,
    /* [out][in] */ SAFEARRAY * *pvArray,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_GetAllAxisPosStr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetMaxFeedrates_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [out] */ double *pMaxPerRev,
    /* [out] */ double *pMaxPerMinute);


void __RPC_STUB IPmacNC_GetMaxFeedrates_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_SetMaxFeedrates_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ double MaxPerRev,
    /* [in] */ double MaxPerMinute);


void __RPC_STUB IPmacNC_SetMaxFeedrates_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_ClearOffsetValue_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [in] */ short axis,
    /* [in] */ NCCOORDTYPE t,
    /* [in] */ VARIANT_BOOL bMetric,
    /* [in] */ double NewVal,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_ClearOffsetValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetToolDataStruct_Proxy( 
    IPmacNC * This,
    /* [in] */ LONG Control,
    /* [in] */ LONG Coord,
    /* [in] */ LONG Tool,
    /* [out][in] */ NCTOOL *pStruct);


void __RPC_STUB IPmacNC_GetToolDataStruct_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_StartDataCollection_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long CollectionMSec,
    /* [in] */ long BufferSize,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_StartDataCollection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_StopDataCollection_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control);


void __RPC_STUB IPmacNC_StopDataCollection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_DataCollect_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long Coord,
    /* [out][in] */ SAFEARRAY * *pvArray,
    /* [out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IPmacNC_DataCollect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_ResetPartsCount_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control);


void __RPC_STUB IPmacNC_ResetPartsCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_ResetPartsTotal_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control);


void __RPC_STUB IPmacNC_ResetPartsTotal_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmacNC_GetWorkOffset_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [in] */ long coord,
    /* [out] */ NCCOORDTYPE *t,
    /* [out] */ long *pExtendedVal);


void __RPC_STUB IPmacNC_GetWorkOffset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_DataCollectionEnabled_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IPmacNC_get_DataCollectionEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPmacNC_get_DataCollectionResolution_Proxy( 
    IPmacNC * This,
    /* [in] */ long Control,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPmacNC_get_DataCollectionResolution_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPmacNC_INTERFACE_DEFINED__ */


#ifndef ___IPmacNCEvents_DISPINTERFACE_DEFINED__
#define ___IPmacNCEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IPmacNCEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IPmacNCEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("32C9ADDE-9F29-4658-8EC3-F1B8395FE19F")
    _IPmacNCEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IPmacNCEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IPmacNCEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IPmacNCEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IPmacNCEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IPmacNCEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IPmacNCEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IPmacNCEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IPmacNCEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IPmacNCEventsVtbl;

    interface _IPmacNCEvents
    {
        CONST_VTBL struct _IPmacNCEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IPmacNCEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IPmacNCEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IPmacNCEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IPmacNCEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IPmacNCEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IPmacNCEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IPmacNCEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IPmacNCEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_PmacNC;

#ifdef __cplusplus

class DECLSPEC_UUID("450149A8-E6B7-4563-8038-FBCAD025D056")
PmacNC;
#endif
#endif /* __PCOMMSERVERLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


