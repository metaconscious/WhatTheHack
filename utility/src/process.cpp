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
    std::cout << __func__ << ": processName = " << processName << '\n';
    DWORD processId{};

    HANDLE hSnapshot{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };

    if (hSnapshot != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 processEntry{ .dwSize = sizeof(processEntry) };

        if (Process32First(hSnapshot, &processEntry))
        {
            do
            {
                std::cout << __func__ << ": Current process name is " << processEntry.szExeFile << ".\n";
                if (isCaseInsensitivelyEqual(processEntry.szExeFile, processName))
                {
                    processId = processEntry.th32ProcessID;
                }
            } while (Process32Next(hSnapshot, &processEntry));
        }
        else
        {
            printErrorMessageIfOccurred(__FILE__, __LINE__, __func__);
        }
    }
    else
    {
        printErrorMessageIfOccurred(__FILE__, __LINE__, __func__);
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
                std::wcout << __func__ << ": Current module name is " << moduleEntry.szModule << '\n';
                if (!_wcsicmp(moduleEntry.szModule, moduleName.data()))
                {
                    modBaseAddr = reinterpret_cast<uintptr_t>(moduleEntry.modBaseAddr);
                    break;
                }
            } while (Module32NextW(hSnap, &moduleEntry));
        }
        else
        {
            printErrorMessageIfOccurred(__FILE__, __LINE__, __func__);
        }
    }
    else
    {
        printErrorMessageIfOccurred(__FILE__, __LINE__, __func__);
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
                std::cout << __func__ << ": Current module name is " << moduleEntry.szModule << '\n';
                if (isCaseInsensitivelyEqual(moduleEntry.szModule, moduleName))
                {
                    modBaseAddr = reinterpret_cast<uintptr_t>(moduleEntry.modBaseAddr);
                    break;
                }
            } while (Module32Next(hSnap, &moduleEntry));
        }
        else
        {
            printErrorMessageIfOccurred(__FILE__, __LINE__, __func__);
        }
    }
    else
    {
        printErrorMessageIfOccurred(__FILE__, __LINE__, __func__);
    }

    CloseHandle(hSnap);
    return modBaseAddr;
}
