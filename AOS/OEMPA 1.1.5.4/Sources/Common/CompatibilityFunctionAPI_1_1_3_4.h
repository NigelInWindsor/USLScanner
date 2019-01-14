#pragma once
//HERE IS MACRO AND INLINE FUNCTION TO SHOW YOU HOW TO GET COMPATIBILITY WITH OLDER VERSION
//ALL MACROS HAVE NOT BEEN IMPLEMENTED BUT THERE IS A LOT OF EXAMPLES, SO YOU CAN IMPLEMENT NEW ONE BY YOURSELF
//But it is better to not use them directly and to rewrite your code, for that you can get inspiration from those macros.

#define myMessageBox					OempaApiMessageBox
#define	GetResolution					lGetResolution
#define	SetResolution					lSetResolution
#define UpdateStatusProcess				UpdateScan
#define OEMPA_SW_ENCODER1_INVERT		0
#define OEMPA_SW_ENCODER2_INVERT		0
#define structFocalLaw					CFocalLaw
#define eReadFileWriteHW_AfterWriteHW	eReadFileWriteHW_Leave

#include "CompatibilityFunctionAPI_1_0_1_1.h"
