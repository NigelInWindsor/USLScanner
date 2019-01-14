
#pragma once

#ifdef UTKERNEL_EXPORTS
#ifndef UTKERNEL_API
#define UTKERNEL_API __declspec(dllexport)
#define UTKERNEL_API_ALIGN __declspec(dllexport) __declspec(align(8))
#endif
#else
#ifndef UTKERNEL_API
#define UTKERNEL_API __declspec(dllimport)
#define UTKERNEL_API_ALIGN __declspec(dllimport) __declspec(align(8))
#endif
#endif

