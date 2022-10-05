#include <iostream>
#include <string>
#include <windows.h>
#include "logger.h"
#include "memory.h"
#include "process.h"

int main(int argc, const char* argv[])
{
    if (argc != 3)
    {
        Error("Incorrect argument number.");
        std::cout << "Usage: WhatTheHack <process-name> <module-name>\n";
        return 1;
    }


    std::string_view processName{ argv[1] };
    DebugVarInfo(processName);

    auto processId{ getProcessId(processName) };
    DebugVarInfo(processId);

    std::string_view moduleName{ argv[2] };
    DebugVarInfo(moduleName);

    auto moduleBaseAddr{ getModuleBaseAddress(processId, moduleName) };
    DebugVarInfoFmt(moduleBaseAddr, std::ios_base::hex, std::ios_base::showbase);

    auto hProcess{ OpenProcess(PROCESS_ALL_ACCESS, 0, processId) };

    auto dynamicPointerBase{ moduleBaseAddr + 0x10F4F4u };
    DebugVarInfoFmt(dynamicPointerBase, std::ios_base::hex, std::ios_base::showbase);

    auto healthAddr{ getMultilevelPointerAddress(hProcess, dynamicPointerBase, { 0xf8 }) };
    DebugVarInfoFmt(healthAddr, std::ios_base::hex, std::ios_base::showbase);

    auto ammoAddr{ getMultilevelPointerAddress(hProcess, dynamicPointerBase, { 0x374, 0x14, 0x0 }) };
    DebugVarInfoFmt(ammoAddr, std::ios_base::hex, std::ios_base::showbase);

    int health{};
    ReadProcessMemory(hProcess, reinterpret_cast<BYTE*>(healthAddr), &health, sizeof(health), nullptr);
    DebugVarInfoFmt(health);

    int ammo{};
    ReadProcessMemory(hProcess, reinterpret_cast<BYTE*>(ammoAddr), &ammo, sizeof(ammo), nullptr);
    DebugVarInfoFmt(ammoAddr);
    
    bool invulnerable{ false };

    DWORD exitCode{};

    while (GetExitCodeProcess(hProcess, &exitCode) && exitCode == STILL_ACTIVE)
    {
        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            if (invulnerable)
            {
                patchOperation(hProcess, moduleBaseAddr + 0x29D1F, "\x29\x7b\x04", 3);
            }
            else
            {
                patchNop(hProcess, moduleBaseAddr + 0x29D1F, 3);
            }
            invulnerable = !invulnerable;
        }
    }

    Debug("Process ended.");

    return 0;
}
