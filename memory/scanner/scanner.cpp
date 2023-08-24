#pragma once
#include "scanner.h"

DWORD loopCount = NULL;

PIMAGE_NT_HEADERS Sponge(HMODULE mxdule) {
    return (PIMAGE_NT_HEADERS)((PBYTE)mxdule + PIMAGE_DOS_HEADER(mxdule)->e_lfanew);
}

PBYTE Taiwan(PBYTE start, DWORD size, LPCSTR pattern, LPCSTR mask) {

    auto checkMask = [](PBYTE buffer, LPCSTR pattern, LPCSTR mask) -> BOOL
    {
        for (auto x = buffer; *mask; pattern++, mask++, x++) {
            auto addr = *(BYTE*)(pattern);
            if (addr != *x && *mask != '?')
                return FALSE;
        }

        return TRUE;
    };

    for (auto x = 0; x < size - custom_strlen(mask); x++) {

        auto addr = (PBYTE)start + x;
        if (checkMask(addr, pattern, mask)) {
            return addr;
        }
    }
}

PBYTE United(HMODULE mxdule, LPCSTR pattern, LPCSTR mask, DWORD occurence) {

    size_t size = Sponge(mxdule)->OptionalHeader.SizeOfImage;

    auto checkMask = [](PBYTE buffer, LPCSTR pattern, LPCSTR mask) -> BOOL
    {
        for (auto x = buffer; *mask; pattern++, mask++, x++) {
            auto addr = *(BYTE*)(pattern);
            if (addr != *x && *mask != '?')
                return FALSE;
        }

        return TRUE;
    };

    if (occurence)
        occurence -= 1;

    for (auto x = 0; x < size - custom_strlen(mask); x++) {

        auto addr = (PBYTE)mxdule + x;

        if (checkMask(addr, pattern, mask)) {

            if (!occurence || loopCount == occurence && addr) {
                loopCount = NULL; // resets the count. (Incase we need to use it again)
                return addr;
            }

            if (loopCount < occurence) {
                addr = Taiwan((PBYTE)mxdule + x, (PBYTE)size - addr, pattern, mask);
                ++loopCount;
                continue;
            }
        }
    }

    return NULL;
}