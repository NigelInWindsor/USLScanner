#ifndef	CTIERRCODES_INCLUDED
#define CTIERRCODES_INCLUDED
/*

  Copyright (c) 2001-2006 Crandun Technologies Inc.  All rights reserved.

  This file MAY NOT BE REDISTRIBUTED, in whole or in part,
  without the express written permission of Crandun Technologies Inc.

  ------------------------------------------------------------------------------
  Programmer's Notes:

  (1) Include this header file in any program that uses
      the Crandun Technologies Inc. software library
      to interface to the Acuity Research Inc. sensors.

*/


#define CTI_SUCCESS 0

#define CTI_ERROR_BASE 0
#define CTI_FAILURE				(CTI_ERROR_BASE -1)
#define CTI_BAD_PARAM			(CTI_ERROR_BASE -2)
#define CTI_BUFFER_TOO_SMALL	(CTI_ERROR_BASE -3)
#define CTI_ILLEGAL_CALL		(CTI_ERROR_BASE -4)
#define CTI_INTERNAL_ERROR		(CTI_ERROR_BASE -5)
#define CTI_THREAD_EXIT			(CTI_ERROR_BASE -6)

#define CTI_NO_MEMORY			(CTI_ERROR_BASE-50)

#define CTI_AR600_ERR_BASE		-60
#define CTI_SPAN_NOT_SET		(CTI_AR600_ERR_BASE-1)


#define CTI_HSIF_ERROR_BASE -100
#define CTI_BUFFER_OVERFLOW		(CTI_HSIF_ERROR_BASE-1)
#define CTI_BUFFER_OVERFLOW_1		(CTI_HSIF_ERROR_BASE-2)
#define CTI_BUFFERSIZE_UNKNOWN	(CTI_HSIF_ERROR_BASE-3)
#define CTI_HSIFOVERFLOW			(CTI_HSIF_ERROR_BASE-4)
#define CTI_BOARDPARAMS_NOT_SET	(CTI_HSIF_ERROR_BASE-5)
#define CTI_CANNOT_OPEN				(CTI_HSIF_ERROR_BASE-6)
#define CTI_HW_INIT_FAILURE		(CTI_HSIF_ERROR_BASE-7)
#define CTI_BUFFER_OVERFLOW_2		(CTI_HSIF_ERROR_BASE-8)
#define CTI_CALIBFILE_NOT_SET		(CTI_HSIF_ERROR_BASE-9)
#define CTI_FILE_READ_ERROR		(CTI_HSIF_ERROR_BASE-10)
#define CTI_CALIBRATION_ERROR		(CTI_HSIF_ERROR_BASE-11)


#define CTI_COMM_ERROR_BASE -200
#define CTI_COMM_ERROR			(CTI_COMM_ERROR_BASE-1)
#define CTI_COMM_TIMEOUT		(CTI_COMM_ERROR_BASE-2)
#define CTI_COMM_OPEN_ERROR	(CTI_COMM_ERROR_BASE-3)
#define CTI_COMM_NOT_OPEN		(CTI_COMM_ERROR_BASE-4)


#define CTI_THREAD_ERROR_BASE -400
#define CTI_THREAD_CREATE_FAILURE	(CTI_THREAD_ERROR_BASE-1)
#define CTI_MUTEX_FAILURE				(CTI_THREAD_ERROR_BASE-2)
#define CTI_EVENT_CREATE_FAILURE		(CTI_THREAD_ERROR_BASE-3)
#define CTI_WAIT_TIMEOUT				(CTI_THREAD_ERROR_BASE-4)

#define CTI_LIC_EXPIRED	-998
#define CTI_LICENSE_ERR	-999



#endif /* CTIERRCODES_INCLUDED */

