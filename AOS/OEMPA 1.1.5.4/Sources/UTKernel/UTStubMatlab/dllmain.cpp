// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

extern "C"{
__declspec(dllexport) VS_FIXEDFILEINFO g_VS_FIXEDFILEINFO={	0xFEEF04BD,//DWORD dwSignature;
												0,//DWORD dwStrucVersion;
												0x00010001,//DWORD dwFileVersionMS;
												0x00050004,//DWORD dwFileVersionLS;
												0x00010001,//DWORD dwProductVersionMS;
												0x00050004,//DWORD dwProductVersionLS;
												VS_FF_DEBUG | VS_FF_PRERELEASE | VS_FF_PATCHED | VS_FF_PRIVATEBUILD | VS_FF_INFOINFERRED | VS_FF_SPECIALBUILD,//DWORD dwFileFlagsMask;
#ifdef _DEBUG
												VS_FF_DEBUG,//DWORD dwFileFlags;
#else _DEBUG
#ifdef _PRERELEASE
												VS_FF_PRERELEASE,//DWORD dwFileFlags;
#else _PRERELEASE
												0,//DWORD dwFileFlags;
#endif _PRERELEASE
#endif _DEBUG
												VOS__WINDOWS32,//DWORD dwFileOS;
												VFT_DLL,//DWORD dwFileType;
												0,//DWORD dwFileSubtype;
												0,//DWORD dwFileDateMS;
												0//DWORD dwFileDateLS;
											};
}

void Init();

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	static HMODULE hModule2=NULL;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		hModule2 = hModule;
		Init();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:	
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
