#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

#include "../GrailLib/Ex.h"
#include "../GrailLib/Util.h"

int main()
{
	std::cout << "dll path:" << std::endl;
    std::string dllPathStr;
	std::cin >> dllPathStr;
    const char* dllPath = dllPathStr.c_str();
	
    DWORD procId = 0;

    while (!procId)
    {
        procId = Ex::FindProcessId(L"ac_client.exe");
        Sleep(30);
    }

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, procId);

    if (hProc && hProc != INVALID_HANDLE_VALUE)
    {
        void* loc = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        WriteProcessMemory(hProc, loc, dllPath, strlen(dllPath) + 1, 0);

        HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);

        if (hThread)
        {
            CloseHandle(hThread);
        }
    }

    if (hProc)
    {
        CloseHandle(hProc);
    }
    return 0;
}
