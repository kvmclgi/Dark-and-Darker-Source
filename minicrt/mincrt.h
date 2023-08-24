#pragma once
#include <Windows.h>
#include <winternl.h>
#include <TlHelp32.h>
#include <cstdlib>
#include <cstdio>
#include <new>

#include "..\importer\lazy_importer.h"
#include "../spoofer/Spoof.h"
//#include "../DriverUtil.h"

int My_wcslen(const wchar_t* string);
//void custom_MemCpy(void* dest, void* src, size_t n);
void* custom_MemCpy(void* dest, void* src, size_t n);
int custom_strlen(const char* string);
PBYTE GetModuleBase_Wrapper(const char* ModuleName);
PVOID GetExportAddress(PBYTE hDll, const char* Name);
DWORD GetPID(const wchar_t* procName);
size_t custom_wcstombs(char* dest, const wchar_t* src, size_t n);
errno_t custom_memcpy_s(void* dest, size_t destSize, void* src, size_t count);
//int my_sprintf(char* str, const char* format, ...);
