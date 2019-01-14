Attribute VB_Name = "CTI_HSIF_PCI_Defs"
Option Explicit

'  Copyright (c) 2006 Crandun Technologies Inc.  All rights reserved.
'
'  Programmer 's Notes:
'  Include this file as a Standard (Code) Module in any
'  Visual Basic or Visual Basic for Applications program
'  using the Crandun Technologies Inc. library
'

' Visual Basic data type for returning sensor data
Public Type HSIF_DATA_PT
   R_X As Single        '  calibrated range value (polar output), or X value of range(cartesian output)
   A1_Y As Single       '  encoder #1 angle (polar output), or Y value of range(cartesian output)
   A2 As Single         '  encoder #2 angle
   rawE1 As Long        '  raw count for encoder #1
   rawE2 As Long        '  raw count for encoder #2
   amplitude As Long    '  signal amplitude reading (0-255)
   ambient As Long      '  ambient light reading (0-255)
   temperature As Single '  sensor internal temperature in degrees F.
   rawRange As Long     '  raw range reading
   inputs As Long       '  lower 3 bits will be 0/1 depending on status of inputs 1,2,3
End Type


Public Const CTI_SUCCESS As Long = 0

Public Const CTI_ERROR_BASE       As Long = 0
Public Const CTI_FAILURE          As Long = -1
Public Const CTI_BAD_PARAM        As Long = -2
Public Const CTI_BUFFER_TOO_SMALL As Long = -3
Public Const CTI_ILLEGAL_CALL     As Long = -4
Public Const CTI_INTERNAL_ERROR   As Long = -5
Public Const CTI_THREAD_EXIT      As Long = -6

Public Const CTI_NO_MEMORY        As Long = -50

Public Const CTI_SPAN_NOT_SET     As Long = -61

Public Const CTI_HSIF_PCI_ERROR_BASE       As Long = -100
Public Const CTI_BUFFER_OVERFLOW       As Long = -101
Public Const CTI_BUFFER_OVERFLOW_1     As Long = -102
Public Const CTI_BUFFERSIZE_UNKNOWN    As Long = -103
Public Const CTI_HSIF_PCIOVERFLOW          As Long = -104
Public Const CTI_BOARDPARAMS_NOT_SET   As Long = -105
Public Const CTI_CANNOT_OPEN           As Long = -106
Public Const CTI_HW_INIT_FAILURE       As Long = -107
Public Const CTI_BUFFER_OVERFLOW_2     As Long = -108
Public Const CTI_CALIBFILE_NOT_SET     As Long = -109
Public Const CTI_FILE_READ_ERROR       As Long = -110
Public Const CTI_CALIBRATION_ERROR     As Long = -111

Public Const CTI_COMM_ERROR_BASE       As Long = -200
Public Const CTI_COMM_ERROR            As Long = -201
Public Const CTI_COMM_TIMEOUT          As Long = -202
Public Const CTI_COMM_OPEN_ERROR       As Long = -203
Public Const CTI_COMM_NOT_OPEN         As Long = -204


Public Const CTI_THREAD_ERROR_BASE     As Long = -400
Public Const CTI_THREAD_CREATE_FAILURE As Long = -401
Public Const CTI_MUTEX_FAILURE         As Long = -402
Public Const CTI_EVENT_CREATE_FAILURE  As Long = -403
Public Const CTI_WAIT_TIMEOUT          As Long = -404

Public Const CTI_LIC_EXPIRED As Long = -998
Public Const CTI_LICENSE_ERR As Long = -999

