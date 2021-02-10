#include "pch.h"
#include "Ex.h"

#include <cstdint>
#include <string>
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

namespace Ex
{
	DWORD FindProcessId(const std::wstring& processName)
	{
		PROCESSENTRY32 processInfo;
		processInfo.dwSize = sizeof(processInfo);

		HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		if (processesSnapshot == INVALID_HANDLE_VALUE)
			return 0;

		Process32First(processesSnapshot, &processInfo);
		if (!processName.compare(processInfo.szExeFile))
		{
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}

		while (Process32Next(processesSnapshot, &processInfo))
		{
			if (!processName.compare(processInfo.szExeFile))
			{
				CloseHandle(processesSnapshot);
				return processInfo.th32ProcessID;
			}
		}

		CloseHandle(processesSnapshot);
		return 0;
	}

	uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName, MODULEENTRY32& ownModEntry)
	{
		uintptr_t modBaseAddr = 0;
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
		if (hSnap != INVALID_HANDLE_VALUE)
		{
			MODULEENTRY32 modEntry;
			modEntry.dwSize = sizeof(modEntry);
			if (Module32First(hSnap, &modEntry))
			{
				do
				{
					if (!_wcsicmp(modEntry.szModule, modName))
					{
						modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
						ownModEntry = modEntry;
						break;
					}
				} while (Module32Next(hSnap, &modEntry));
			}
		}
		CloseHandle(hSnap);
		return modBaseAddr;
	}

	uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets)
	{
		uintptr_t addr = ptr;
		for (unsigned int i = 0; i < offsets.size(); ++i)
		{
			ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
			addr += offsets[i];
		}
		return addr;
	}

	void Patch(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess)
	{
		DWORD oldprotect;
		VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
		WriteProcessMemory(hProcess, dst, src, size, nullptr);
		VirtualProtectEx(hProcess, dst, size, oldprotect, &oldprotect);
	}

	void Nop(BYTE* dst, unsigned int size, HANDLE hProcess)
	{
		BYTE* nopArray = new BYTE[size];
		memset(nopArray, 0x90, size);

		Patch(dst, nopArray, size, hProcess);
		delete[] nopArray;
	}
}
