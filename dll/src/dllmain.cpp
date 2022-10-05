//
// Created by user on 10/2/2022.
//

#include <windows.h>
#include <cstdio>
#include <iostream>
#include "logger.h"
#include "memory.h"

void hackMain(uintptr_t moduleBaseAddress)
{
    auto localPlayerAddress{ moduleBaseAddress + 0x10f4f4 };
    auto healthPtr{ reinterpret_cast<int*>(getMultilevelPointerAddress(localPlayerAddress, { 0xf8 })) };
    auto ammoPtr{ reinterpret_cast<int*>(getMultilevelPointerAddress(localPlayerAddress, { 0x374, 0x14, 0x0 })) };

    Patcher recoilPatcher{ moduleBaseAddress + 0x63786 };

    bool infiniteHealing{};
    bool infiniteAmmo{};
    const auto& noRecoil{ recoilPatcher.patched() };

    while (true)
    {
        if (GetAsyncKeyState(VK_END) & 1)
        {
            break;
        }

        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            infiniteHealing = !infiniteHealing;
        }

        if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        {
            infiniteAmmo = !infiniteAmmo;
        }

        if (GetAsyncKeyState(VK_NUMPAD3) & 1)
        {
            if (noRecoil)
            {
                recoilPatcher.restore();
            }
            else
            {
                recoilPatcher.patch(10);
            }
        }

        if (infiniteHealing)
        {
            *healthPtr = 1337;
        }

        if (infiniteAmmo)
        {
            *ammoPtr = 1337;
        }

        Sleep(5);
    }
}

DWORD WINAPI hackThread(HMODULE hModule)
{
    FILE* file{ nullptr };
    if (AllocConsole())
    {
        freopen_s(&file, "CONOUT$", "w", stdout);
    }

    std::cout << "Test string\n";

    auto moduleBaseAddress{ reinterpret_cast<uintptr_t>(GetModuleHandle("ac_client.exe")) };

    if (moduleBaseAddress != 0)
    {
        DebugVarInfoFmt(moduleBaseAddress, std::ios_base::hex, std::ios_base::showbase);
        // !!! Separate RAII code in a new scope, cuz current function will exit without calling destructors!
        // !!! like some function like hackMain() below!
        // !!! or just wrapping the code with an enclosed curly brace!
        hackMain(moduleBaseAddress);
    }

    std::fclose(file);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, EXIT_SUCCESS);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpvReserved)
{
    // Perform actions based on the reason for calling.
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        // Initialize once for each new process.
        // Return FALSE to fail DLL load.
        CloseHandle(CreateThread(nullptr,
            0,
            reinterpret_cast<LPTHREAD_START_ROUTINE>(hackThread),
            hModule,
            0,
            nullptr));
        break;

    case DLL_THREAD_ATTACH:
        // Do thread-specific initialization.
        break;

    case DLL_THREAD_DETACH:
        // Do thread-specific cleanup.
        break;

    case DLL_PROCESS_DETACH:

        if (lpvReserved != nullptr)
        {
            break; // do not do cleanup if process termination scenario
        }

        // Perform any necessary cleanup.
        break;
    default:
        break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}