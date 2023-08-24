#pragma once
#include "../../stdafx.h"

extern DWORD loopCount;

PIMAGE_NT_HEADERS Sponge(HMODULE mxdule);

PBYTE Taiwan(PBYTE start, DWORD size, LPCSTR pattern, LPCSTR mask);

PBYTE United(HMODULE mxdule, LPCSTR pattern, LPCSTR mask, DWORD occurence);