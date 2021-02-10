#pragma once

#include <vector>
#include <Windows.h>

namespace In
{
	uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);

	void Patch(BYTE* dst, BYTE* src, unsigned int size);
	void Nop(BYTE* dst, unsigned int size);
	bool Hook(void* toHook, void* ourFunc, int len);
	BYTE* TrampHook(void* toHook, void* ourFunc, int len);

	struct Hook
	{
		bool bStatus{ false };

		BYTE* src{ nullptr };
		BYTE* dst{ nullptr };
		BYTE* PtrToGatewayFnPtr{ nullptr };
		uintptr_t len{ 0 };

		BYTE originalBytes[10]{ 0 };

		Hook(BYTE* src, BYTE* dst, BYTE* PtrToGatewayPtr, uintptr_t len);
		Hook(const char* exportName, const char* modName, BYTE* dst, BYTE* PtrToGatewayPtr, uintptr_t len);

		void Enable();
		void Disable();
		void Toggle();
	};
}