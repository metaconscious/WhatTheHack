//
// Created by user on 10/2/2022.
//
#include "process.h"
#include <tlhelp32.h>
#include <iostream>
#include "logger.h"
#include "utils.h"

DWORD getProcessId(std::string_view processName)
{
    DebugVarInfo(processName);
    DWORD processId{};

    HANDLE hSnapshot{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };

    if (hSnapshot != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 processEntry{ .dwSize = sizeof(processEntry) };

        if (Process32First(hSnapshot, &processEntry))
        {
            do
            {
                DebugVarInfo(processEntry.szExeFile);
                if (isCaseInsensitivelyEqual(processEntry.szExeFile, processName))
                {
                    processId = processEntry.th32ProcessID;
                }
            } while (Process32Next(hSnapshot, &processEntry));
        }
        else
        {
            auto code{ static_cast<int>(GetLastError()) };
            SystemError(code);
            exit(code);
        }
    }
    else
    {
        auto code{ static_cast<int>(GetLastError()) };
        SystemError(code);
        exit(code);
    }

    CloseHandle(hSnapshot);
    return processId;
}

HANDLE getProcessHandleByName(std::string_view processName)
{
    auto processId{ getProcessId(processName) };
    auto hProcess{ OpenProcess(PROCESS_ALL_ACCESS, false, processId) };
    return hProcess;
}

uintptr_t getModuleBaseAddress(DWORD procId, std::wstring_view moduleName)
{
    uintptr_t modBaseAddr{};

    HANDLE hSnap{ CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId) };

    if (hSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32W moduleEntry{ .dwSize = sizeof(moduleEntry) };

        if (Module32FirstW(hSnap, &moduleEntry))
        {
            do
            {
                if (!_wcsicmp(moduleEntry.szModule, moduleName.data()))
                {
                    modBaseAddr = reinterpret_cast<uintptr_t>(moduleEntry.modBaseAddr);
                    break;
                }
            } while (Module32NextW(hSnap, &moduleEntry));
        }
        else
        {
            auto code{ static_cast<int>(GetLastError()) };
            SystemError(code);
            exit(code);
        }
    }
    else
    {
        auto code{ static_cast<int>(GetLastError()) };
        SystemError(code);
        exit(code);
    }

    CloseHandle(hSnap);
    return modBaseAddr;
}

uintptr_t getModuleBaseAddress(DWORD procId, std::string_view moduleName)
{
    uintptr_t modBaseAddr{};

    HANDLE hSnap{ CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId) };

    if (hSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 moduleEntry{ .dwSize = sizeof(moduleEntry) };

        if (Module32First(hSnap, &moduleEntry))
        {
            do
            {
                DebugVarInfo(moduleEntry.szModule);
                if (isCaseInsensitivelyEqual(moduleEntry.szModule, moduleName))
                {
                    modBaseAddr = reinterpret_cast<uintptr_t>(moduleEntry.modBaseAddr);
                    break;
                }
            } while (Module32Next(hSnap, &moduleEntry));
        }
        else
        {
            auto code{ static_cast<int>(GetLastError()) };
            SystemError(code);
            exit(code);
        }
    }
    else
    {
        auto code{ static_cast<int>(GetLastError()) };
        SystemError(code);
        exit(code);
    }

    CloseHandle(hSnap);
    return modBaseAddr;
}
