#include <iostream>
#include <string>
#include <windows.h>
#include "utils.h"
#include "memory.h"
#include "process.h"

int main(int argc, const char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Incorrect argument number.\n";
        std::cout << "Usage: WhatTheHack <process-name> <module-name>\n";
        return 1;
    }
    std::cout << __func__ << ": argv[0] = " << argv[0] << '\n';
    std::string_view processName{ argv[1] };
    std::cout << __func__ << ": processName = " << processName << '\n';

    auto processId{ getProcessId(processName) };
    std::cout << __func__ << ": processId = " << processId << '\n';

    std::string_view moduleName{ argv[2] };
    std::cout << __func__ << ": moduleName = " << processName << '\n';

    auto moduleBaseAddr{ getModuleBaseAddress(processId, moduleName) };

    std::cout << "The base address of " << moduleName << " is " << std::hex << std::showbase << moduleBaseAddr << ".\n";

    auto hProcess{ OpenProcess(PROCESS_ALL_ACCESS, 0, processId) };

    auto dynamicPointerBase{ moduleBaseAddr + 0x10F4F4u };
    std::cout << __func__ << ": dynamicPointerBase = " << std::hex << std::showbase << dynamicPointerBase << '\n';

    auto healthAddr{ getMultilevelPointerAddress(hProcess, dynamicPointerBase, { 0xf8 }) };
    std::cout << __func__ << ": healthAddr = " << std::hex << std::showbase << healthAddr << '\n';

    auto ammoAddr{ getMultilevelPointerAddress(hProcess, dynamicPointerBase, { 0x374, 0x14, 0x0 }) };
    std::cout << __func__ << ": ammoAddr = " << std::hex << std::showbase << ammoAddr << '\n';

    int health{};
    ReadProcessMemory(hProcess, reinterpret_cast<BYTE*>(healthAddr), &health, sizeof(health), nullptr);
    std::cout << __func__ << ": Current Health is " << std::dec << health << '\n';

    int ammo{};
    ReadProcessMemory(hProcess, reinterpret_cast<BYTE*>(ammoAddr), &ammo, sizeof(ammo), nullptr);
    std::cout << __func__ << ": Current Ammo is " << std::dec << ammo << '\n';
    
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

    std::cout << "Process End.\n";

    return 0;
}
