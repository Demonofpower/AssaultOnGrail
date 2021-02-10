#include "pch.h"
#include "In.h"

namespace In
{
	uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets)
	{
		uintptr_t addr = ptr;
		for (unsigned int i = 0; i < offsets.size(); ++i)
		{
			addr = *(uintptr_t*)addr;
			addr += offsets[i];
		}
		return addr;
	}

	void Patch(BYTE* dst, BYTE* src, unsigned int size)
	{
		DWORD oldprotect;
		VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
		memcpy(dst, src, size);
		VirtualProtect(dst, size, oldprotect, &oldprotect);
	}

	void Nop(BYTE* dst, unsigned int size)
	{
		DWORD oldprotect;
		VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
		memset(dst, 0x90, size);
		VirtualProtect(dst, size, oldprotect, &oldprotect);
	}

	bool Hook(void* toHook, void* ourFunc, int len) {
		if (len < 5) {
			return false;
		}

		DWORD curProtection;
		VirtualProtect(toHook, len, PAGE_EXECUTE_READWRITE, &curProtection);

		memset(toHook, 0x90, len);

		DWORD relativeAddress = ((DWORD)ourFunc - (DWORD)toHook) - 5;

		*(BYTE*)toHook = 0xE9;
		*(DWORD*)((DWORD)toHook + 1) = relativeAddress;

		DWORD temp;
		VirtualProtect(toHook, len, curProtection, &temp);

		return true;
	}

	BYTE* TrampHook(void* toHook, void* ourFunc, int len)
	{
		if (len < 5) {
			return 0;
		}

		BYTE* gateway = (BYTE*) VirtualAlloc(0, len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

		memcpy_s(gateway, len, (BYTE*) toHook, len);

		uintptr_t gatewayRelativeAddr = (BYTE*) toHook - gateway - 5;

		*(BYTE*)(gateway + len) = 0xE9;
		*(DWORD*)((DWORD)gateway + len + 1) = gatewayRelativeAddr;

		Hook(toHook, ourFunc, len);

		return gateway;
	}

	Hook::Hook(BYTE* src, BYTE* dst, BYTE* PtrToGatewayFnPtr, uintptr_t len)
	{
		this->src = src;
		this->dst = dst;
		this->len = len;
		this->PtrToGatewayFnPtr = PtrToGatewayFnPtr;
	}

	Hook::Hook(const char* exportName, const char* modName, BYTE* dst, BYTE* PtrToGatewayFnPtr, uintptr_t len)
	{
		HMODULE hMod = GetModuleHandleA(modName);

		this->src = (BYTE*)GetProcAddress(hMod, exportName);
		this->dst = dst;
		this->len = len;
		this->PtrToGatewayFnPtr = PtrToGatewayFnPtr;
	}

	void Hook::Enable()
	{
		memcpy(originalBytes, src, len);
		*(uintptr_t*)PtrToGatewayFnPtr = (uintptr_t)TrampHook(src, dst, len);
		bStatus = true;
	}

	void Hook::Disable()
	{
		Patch(src, originalBytes, len);
		bStatus = false;
	}

	void Hook::Toggle()
	{
		if (!bStatus) Enable();
		else Disable();
	}
}