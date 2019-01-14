#ifndef AR4000_DATA_PT_DEF_INCLUDED
#define AR4000_DATA_PT_DEF_INCLUDED

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

typedef struct {
	float range;		/* calibrated range value */
	long amplitude;		/* signal amplitude reading (0-255) */
	long ambient;		/* ambient light reading (0-255) */
	float temperature;	/* sensor internal temperature in degrees F. */
	long rawRange;		/* raw (uncalibrated) range reading */
} AR4000_DATA_PT;


#pragma pack(pop)

#ifdef _cplusplus
}	/* end of namespace */
#endif

#endif /* AR4000_DATA_PT_DEF_INCLUDED */
