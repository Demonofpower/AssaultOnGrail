#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
#include <vector>

#include "../GrailLib/Ex.h"
#include "../GrailLib/Util.h"

void godmode(bool god);
void unlimitedAmmo(bool infiniteAmmo);
void noRecoil(bool no_recoil_enabled);

bool end = false;
bool debugEnabled = false;
bool godEnabled = false;
bool unlimitedAmmoEnabled = false;
bool noRecoilEnabled = false;

HANDLE hProcess;
uintptr_t moduleBase;
uintptr_t playerPtr;

uintptr_t pitchOnePtr;
uintptr_t pitchTwoPtr;
uintptr_t walkBkwdsPtr;

void print()
{
	system("cls");

	if (debugEnabled)
	{
		std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << playerPtr << std::endl;
	}

	std::cout << "Godmode:" << Util::boolToString(godEnabled) << std::endl;
	std::cout << "UnlimitedAmmo:" << Util::boolToString(unlimitedAmmoEnabled) << std::endl;
	std::cout << "NoRecoil:" << Util::boolToString(noRecoilEnabled) << std::endl;
}

void init()
{
	DWORD procId = Ex::FindProcessId(L"ac_client.exe");

	MODULEENTRY32W ownModuleEntry;
	moduleBase = Ex::GetModuleBaseAddress(procId, L"ac_client.exe", ownModuleEntry);

	hProcess = nullptr;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

	playerPtr = moduleBase + 0x10f4f4;
}


void getKeyStates()
{
	if (GetAsyncKeyState(VK_END) & 1)
	{
		end = true;
	}
	if (GetAsyncKeyState(VK_F1) & 1)
	{
		godEnabled = !godEnabled;
		print();
	}
	if (GetAsyncKeyState(VK_F2) & 1)
	{
		unlimitedAmmoEnabled = !unlimitedAmmoEnabled;
		print();
	}
	if (GetAsyncKeyState(VK_F3) & 1)
	{
		noRecoilEnabled = !noRecoilEnabled;
		noRecoil(noRecoilEnabled);
		print();
	}
}

int main()
{
	init();
	print();
	
	while (!end)
	{
		Sleep(100);
		if (debugEnabled)
		{
			print();
		}
		getKeyStates();

		godmode(godEnabled);
		unlimitedAmmo(unlimitedAmmoEnabled);
	}

	return 0;
}

void godmode(bool god)
{
	if (!god)
	{
		return;
	}

	std::vector<unsigned int> healthOffset = {0xF8};
	uintptr_t healthAddr = Ex::FindDMAAddy(hProcess, playerPtr, healthOffset);

	int health = 6969;
	WriteProcessMemory(hProcess, (BYTE*)healthAddr, &health, sizeof(health), nullptr);
}

void unlimitedAmmo(bool infiniteAmmo)
{
	if (!infiniteAmmo)
	{
		return;
	}

	std::vector<unsigned int> healthOffset = {0x374, 0x10, 0x28};
	uintptr_t healthAddr = Ex::FindDMAAddy(hProcess, playerPtr, healthOffset);

	int ammo = 6969;
	WriteProcessMemory(hProcess, (BYTE*)healthAddr, &ammo, sizeof(ammo), nullptr);
}

void noRecoil(bool enable)
{
	if(enable)
	{
		Ex::Patch((BYTE*)(moduleBase + 0x622AC), (BYTE*)"\x90\x90\x90\x5E\x5B", 5, hProcess);
		Ex::Patch((BYTE*)(moduleBase + 0x621E4), (BYTE*)"\x90\x90\x90\x5F\x5E", 5, hProcess);
		Ex::Patch((BYTE*)(moduleBase + 0x6223E), (BYTE*)"\xEB\x10\x90", 3, hProcess);
	}
	else
	{
		Ex::Patch((BYTE*)(moduleBase + 0x622AC), (BYTE*)"\xD9\x5A\x4C\x5E\x5B", 5, hProcess);
		Ex::Patch((BYTE*)(moduleBase + 0x621E4), (BYTE*)"\xD9\x5A\x4C\x5F\x5E", 5, hProcess);
		Ex::Patch((BYTE*)(moduleBase + 0x6223E), (BYTE*)"\xD8\x40\x10", 3, hProcess);
	}
}