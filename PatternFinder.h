#pragma once
#include <Windows.h>
#include <vector>
#include <cstdint>
#include <tlhelp32.h>

auto GetModuleSize(const char* m)
{
	HANDLE hSnap;
	auto processID = GetCurrentProcessId();
	MODULEENTRY32 xModule;
	hSnap = CreateToolhelp32Snapshot(8, processID);
	xModule.dwSize = sizeof(MODULEENTRY32);
	WCHAR wcharname[256];
	memset(wcharname, 0, 512);
	OemToCharW((LPSTR)m,wcharname);
	if (Module32First(hSnap, &xModule))
	{
		while (Module32Next(hSnap, &xModule))
		{
			if (!lstrcmpW(xModule.szModule, wcharname))
			{
				CloseHandle(hSnap);
				return (DWORD)xModule.modBaseSize;
			}
		}
	}
	CloseHandle(hSnap);
	return (DWORD)0;
};

bool __GetLibraryInformation(const char* library, uintptr_t* address, size_t* size)
{
#ifdef WIN32

	* address = (uintptr_t)GetModuleHandleA(library);
	*size = GetModuleSize(library);
	return *address && *size;
#else

	if (libraries.size() == 0)
	{
		dl_iterate_phdr([](struct dl_phdr_info* info, size_t, void*) {
			dlinfo_t library_info = {};

			library_info.library = info->dlpi_name;
			library_info.address = info->dlpi_addr + info->dlpi_phdr[0].p_vaddr;
			library_info.size = info->dlpi_phdr[0].p_memsz;

			libraries.push_back(library_info);

			return 0;
			},
			nullptr);
	}

	for (const dlinfo_t& current : libraries)
	{
		if (!strcasestr(current.library, library))
			continue;

		if (address)
			*address = current.address;

		if (size)
			*size = current.size;

		return true;
	}

	return false;
#endif
}

// original code by dom1n1k and Patrick at GameDeception
inline bool Compare(const unsigned char* pData, const unsigned char* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)
			return false;

	return (*szMask) == 0;
}

namespace PatternFinder
{
	/*
		 * @brief Scan for a given byte pattern on a module
		 *
		 * @param module    Base of the module to search
		 * @param signature IDA-style byte array pattern
		 *
		 * @returns Address of the first occurence
		 * Thanks MarkHC
		 */
	std::uint8_t* PatternScan(const char* module, const char* signature)
	{
		static auto pattern_to_byte = [](const char* pattern) {
			auto bytes = std::vector<int>{};
			auto start = const_cast<char*>(pattern);
			auto end = const_cast<char*>(pattern) + strlen(pattern);

			for (auto current = start; current < end; ++current)
			{
				if (*current == '?')
				{
					++current;
					if (*current == '?')
						++current;
					bytes.push_back(-1);
				}
				else
				{
					bytes.push_back(strtoul(current, &current, 16));
				}
			}
			return bytes;
		};

		uintptr_t moduleptr = 0;
		size_t sizeOfImage = 0;

		__GetLibraryInformation(module, &moduleptr, &sizeOfImage);

		auto patternBytes = pattern_to_byte(signature);
		auto scanBytes = reinterpret_cast<std::uint8_t*>(moduleptr);

		auto s = patternBytes.size();
		auto d = patternBytes.data();
		auto var = sizeOfImage - s;
		for (auto i = 0ll; i < var; ++i)
		{
			bool found = true;
			for (auto j = 0ul; j < s; ++j)
			{
				if (scanBytes[i + j] != d[j] && d[j] != -1)
				{
					found = false;
					break;
				}
			}
			if (found)
			{
				return &scanBytes[i];
			}
		}
		return nullptr;
	}
}