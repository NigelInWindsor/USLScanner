#ifndef HSIF_DATA_PT_DEF_INCLUDED
#define HSIF_DATA_PT_DEF_INCLUDED

/*

  Copyright (c) 2001 Crandun Technologies Inc.  All rights reserved.

  This file MAY NOT BE REDISTRIBUTED, in whole or in part,
  without the express written permission of Crandun Technologies Inc.

*/

/* Define structure used to return data to users of the library */

#ifdef _cplusplus
namespace Crandun {
#endif

#pragma pack(push)		 /* must pack this structures on 1 byte boundary so that VB can read it */
#pragma pack(1)

/* C and C++ structure for returning high-speed interface data */
typedef struct {
	float R_X;			/* calibrated range value (polar output), or X value of range(cartesian output) */
	float A1_Y;			/* encoder #1 angle (polar output), or Y value of range(cartesian output) */
	float A2;			/* encoder #2 angle		 */
	long  rawE1;		/* raw count for encoder #1 */
	long  rawE2;		/* raw count for encoder #2 */
	long  amplitude;	/* signal amplitude reading (0-255) */
	long  ambient;		/* ambient light reading (0-255) */
	float temperature;	/* sensor internal temperature in degrees F. */
	unsigned long  rawRange;		/* raw range reading */	
	long  inputs;		/* lower 3 bits will be 0/1 depending on status of inputs 1,2,3 */
} HSIF_DATA_PT;


#pragma pack(pop)

#ifdef _cplusplus
}	/* end of namespace */
#endif

#endif /* HSIF_DATA_PT_DEF_INCLUDED */
