#pragma once

// Callum Mackenzie

#ifdef _WIN32
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
#include <string>

namespace windows {
#define FILE_PATH_MAX_CHARS 256
	inline const char* fileAbsolutePathA(const char* relativePath) 
	{
		char str[FILE_PATH_MAX_CHARS] = { 0 };
		GetFullPathNameA(relativePath, FILE_PATH_MAX_CHARS, str, NULL);
		return str;
	};
	inline const wchar_t* fileAbsolutePathW(const wchar_t* relativePath)
	{
		wchar_t str[FILE_PATH_MAX_CHARS] = { 0 };
		GetFullPathName(relativePath, FILE_PATH_MAX_CHARS, str, NULL);
		return str;
	};
	inline std::wstring fileAbsolutePathW(std::wstring relPath) 
	{
		wchar_t str[FILE_PATH_MAX_CHARS] = { 0 };
		GetFullPathName(relPath.c_str(), FILE_PATH_MAX_CHARS, str, NULL);
		std::wstring fin(str);
		return fin;
	};
};
#endif