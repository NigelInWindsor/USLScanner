
#pragma once

#ifdef UTKERNEL_DRIVER_EXPORTS
#ifndef UTKERNEL_DRIVER_API
#define UTKERNEL_DRIVER_API __declspec(dllexport)
#define UTKERNEL_DRIVER_API_ALIGN __declspec(dllexport) __declspec(align(8))
#endif
#else
#ifndef UTKERNEL_DRIVER_API
#define UTKERNEL_DRIVER_API __declspec(dllimport)
#define UTKERNEL_DRIVER_API_ALIGN __declspec(dllimport) __declspec(align(8))
#endif
#endif

