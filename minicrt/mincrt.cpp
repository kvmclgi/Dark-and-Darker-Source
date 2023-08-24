#include "mincrt.h"

// This gives Object Error in Main
extern "C" int _fltused = 0;
#pragma data_seg()
#pragma comment(linker, "/merge:.CRT=.rdata")

#pragma intrinsic(memset) // weird fix to compile bug in release
extern void* __cdecl memset(void*, int, size_t);
#pragma function(memset)
void* memset(void* dest, int c, size_t count) {
	unsigned char* p = (unsigned char*)dest;
	for (size_t i = 0; i < count; i++) {
		p[i] = (unsigned char)c;
	}
	return dest;
}

//int my_sprintf(char* str, const char* format, ...)
//{
//	if (!str || !format)
//	{
//		return -1;  // return error if str or format is NULL
//	}
//
//	va_list args;
//	va_start(args, format);
//
//	int i = 0;
//	while (*format)
//	{
//		if (*format == '%')
//		{
//			switch (*(format + 1))
//			{
//			case 'd':
//			{
//				int d = va_arg(args, int);
//				if (d == 0)
//				{
//					*(str + i++) = '0';  // handle case where d is zero
//				}
//				else if (d < 0)
//				{
//					*(str + i++) = '-';
//					d = -d;
//				}
//				char buffer[32];
//				int j = 0;
//				while (d > 0)
//				{
//					buffer[j++] = d % 10 + '0';
//					d /= 10;
//				}
//				while (j > 0)
//				{
//					*(str + i++) = buffer[--j];
//				}
//				format += 2;
//				break;
//			}
//			case 's':
//			{
//				char* s = va_arg(args, char*);
//				if (s == NULL)
//				{
//					*(str + i++) = '(', * (str + i++) = 'n', * (str + i++) = 'u', * (str + i++) = 'l', * (str + i++) = 'l', * (str + i++) = ')';
//				}
//				else
//				{
//					while (*s)
//					{
//						*(str + i++) = *s++;
//					}
//				}
//				format += 2;
//				break;
//			}
//			default:
//			{
//				*(str + i++) = *format++;
//				break;
//			}
//			}
//		}
//		else
//		{
//			*(str + i++) = *format++;
//		}
//	}
//
//	*(str + i) = '\0';
//	va_end(args);
//
//	return i;
//}

size_t custom_wcstombs(char* dest, const wchar_t* src, size_t n) {
	size_t i = 0;
	while (src[i] != L'\0' && i < n) {
		dest[i] = static_cast<char>(src[i]);
		++i;
	}
	if (i < n) {
		dest[i] = '\0';
	}
	return i;
}

int custom_strlen(const char* string)
{
	int cnt = 0;
	if (string)
	{
		for (; *string != 0; ++string) ++cnt;
	}
	return cnt;
}



//void custom_MemCpy(void* dest, void* src, size_t n)
//{
//	char* csrc = (char*)src;
//	char* cdest = (char*)dest;
//
//	for (int i = 0; i < n; i++)
//		cdest[i] = csrc[i];
//}
void* custom_MemCpy(void* dest, void* src, size_t n)
{
	char* csrc = (char*)src;
	char* cdest = (char*)dest;

	for (size_t i = 0; i < n; i++)
		cdest[i] = csrc[i];

	return dest;
}
errno_t custom_memcpy_s(void* dest, size_t destSize, void* src, size_t count) {
	if (dest == nullptr || src == nullptr || destSize < count) {
		return EINVAL;
	}
	custom_MemCpy(dest, src, count);
	return 0;
}

int My_wcslen(const wchar_t* string)
{
	int cnt = 0;
	if (string)
	{
		for (; *string != 0; ++string) ++cnt;
	}
	return cnt;
}
int _wcsicmp(const wchar_t* s1, const wchar_t* s2) {
	while (*s1 && *s2) {
		wchar_t c1 = *s1;
		wchar_t c2 = *s2;
		if (c1 >= 'A' && c1 <= 'Z') {
			c1 += 'a' - 'A';
		}
		if (c2 >= 'A' && c2 <= 'Z') {
			c2 += 'a' - 'A';
		}
		if (c1 != c2) {
			return c1 - c2;
		}
		++s1;
		++s2;
	}
	return *s1 - *s2;
}

#define ToLower(Char) ((Char >= 'A' && Char <= 'Z') ? (Char + 32) : Char)

template <typename StrType, typename StrType2>
bool StrCmp(StrType Str, StrType2 InStr, bool Two) {
	if (!Str || !InStr) return false;
	wchar_t c1, c2; do {
		c1 = *Str++; c2 = *InStr++;
		c1 = ToLower(c1); c2 = ToLower(c2);
		if (!c1 && (Two ? !c2 : 1)) return true;
	} while (c1 == c2); return false;
}

PBYTE GetModuleBase_Wrapper(const char* ModuleName) {
	PPEB_LDR_DATA Ldr = ((PTEB)__readgsqword(FIELD_OFFSET(NT_TIB, Self)))->ProcessEnvironmentBlock->Ldr; void* ModBase = nullptr;
	for (PLIST_ENTRY CurEnt = Ldr->InMemoryOrderModuleList.Flink; CurEnt != &Ldr->InMemoryOrderModuleList; CurEnt = CurEnt->Flink) {
		LDR_DATA_TABLE_ENTRY* pEntry = CONTAINING_RECORD(CurEnt, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
		PUNICODE_STRING BaseDllName = (PUNICODE_STRING)&pEntry->Reserved4[0];
		if (!ModuleName || StrCmp(ModuleName, BaseDllName->Buffer, false))
			return (PBYTE)pEntry->DllBase;
	}
	return nullptr;
}

PVOID GetExportAddress(PBYTE hDll, const char* Name)
{
	//process image data
	PIMAGE_NT_HEADERS NT_Head = (PIMAGE_NT_HEADERS)(hDll + ((PIMAGE_DOS_HEADER)hDll)->e_lfanew);
	PIMAGE_EXPORT_DIRECTORY ExportDir = (PIMAGE_EXPORT_DIRECTORY)(hDll + NT_Head->OptionalHeader.DataDirectory[0].VirtualAddress);

	//process list
	for (DWORD i = 0; i < ExportDir->NumberOfNames; i++)
	{
		//get ordinal & name
		USHORT Ordinal = ((USHORT*)(hDll + ExportDir->AddressOfNameOrdinals))[i];
		const char* ExpName = (const char*)hDll + ((DWORD*)(hDll + ExportDir->AddressOfNames))[i];
		if (StrCmp(Name, ExpName, true))
			return (PVOID)(hDll + ((DWORD*)(hDll + ExportDir->AddressOfFunctions))[Ordinal]);
	}

	return nullptr;
}


DWORD GetPID(const wchar_t* procName)
{
	DWORD procId = 0;
	HANDLE hSnap = SpoofCaller(LI_FN(CreateToolhelp32Snapshot).get(), TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32W procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (SpoofCaller(LI_FN(Process32FirstW).get(), hSnap, &procEntry))
		{
			do
			{
				if (!_wcsicmp(procEntry.szExeFile, procName))
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (SpoofCaller(LI_FN(Process32NextW).get(), hSnap, &procEntry));

		}
	}
	SpoofCaller(LI_FN(CloseHandle).get(), hSnap);
	return procId;
}