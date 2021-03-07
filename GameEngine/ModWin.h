#pragma once

// Callum Mackenzie

#define _WIN32_WINNT 0x0601
#include <sdkddkver.h>

#ifndef FULL_WINTARD
#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
//#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMEMMGR
#define NOMETAFILE
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE
#endif

#define NOMINMAX

#define STRICT

#include <Windows.h>

#ifndef UNICODE
#define UNICODE
#endif

#include "C_WICImageFactory.h"

namespace windows {
	const char* fileAbsolutePathA(const char* relativePath) 
	{
		char str[512];
		GetFullPathNameA(relativePath, 512, str, NULL);
		return str;
	};
	const wchar_t* fileAbsolutePathW(const wchar_t* relativePath)
	{
		wchar_t str[512];
		GetFullPathName(relativePath, 512, str, NULL);
		return str;
	};
};