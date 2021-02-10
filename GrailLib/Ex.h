#pragma once

#include <cstdint>
#include <string>
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

namespace Ex
{
	DWORD FindProcessId(const std::wstring& processName);
	uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName, MODULEENTRY32& ownModEntry);
	uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);

	void Patch(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess);
	void Nop(BYTE* dst, unsigned int size, HANDLE hProcess);
}