#ifndef __GLOBAL_VARS_H
#define __GLOBAL_VARS_H

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
 *      GlobalVars.h
 *
 * Description:
 *
 *      Header for global variables definitions
 *
 * Revision History:
 *
 *      12-01-07 : PLX SDK v5.20
 *
 ******************************************************************************/


#include "DrvDefs.h"




/**********************************************
 *               Globals
 *********************************************/
extern LONG                 Gbl_DeviceCount;
extern ULONG                Gbl_CommonBufferSize;
extern BOOLEAN              Gbl_WindowsVistaOrHigher;
extern PLX_PHYS_MEM_OBJECT *pGbl_CommonBuffer;



#endif
