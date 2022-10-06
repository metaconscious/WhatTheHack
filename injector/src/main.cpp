//
// Created by user on 10/4/2022.
//
#include <windows.h>
#include <iostream>
#include <string_view>
#include "logger.h"
#include "process.h"
#include "utils.h"

int main(int argc, const char* argv[])
{
    if (argc != 3)
    {
        Error("Incorrect argument number.");
        std::cout << "Usage: Injector <process-name> <dll-path>\n";
    }

    std::string_view processName{ argv[1] };
    auto dllPath{ parsePath(argv[2]) };

    auto hProcess{ getProcessHandleByName(processName) };

    if (hProcess == nullptr)
    {
        Error("NULL process handle.");
        return 1;
    }

    if (hProcess == INVALID_HANDLE_VALUE)
    {
        Error("Invalid handle value.");
        return 2;
    }

    auto location{ VirtualAllocEx(hProcess, nullptr, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE) };

    if (location == nullptr)
    {
        Error("Null location pointer.");
        return 3;
    }

    WriteProcessMemory(hProcess, location, dllPath.u8string().c_str(), dllPath.u8string().length() + 1, nullptr);

    auto hThread{ CreateRemoteThread(hProcess,
        nullptr,
        0,
        reinterpret_cast<LPTHREAD_START_ROUTINE>(LoadLibraryA),
        location,
        0,
        nullptr) };

    if (hThread == nullptr)
    {
        Error("NULL thread handle.");
        return 4;
    }

    CloseHandle(hThread);
    VirtualFreeEx(hProcess, location, MAX_PATH, MEM_RELEASE);
    CloseHandle(hProcess);

    return 0;
}