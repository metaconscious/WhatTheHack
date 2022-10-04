//
// Created by user on 10/2/2022.
//

#ifndef WHATTHEHACK_UTILITY_INCLUDE_PROCESS_H_
#define WHATTHEHACK_UTILITY_INCLUDE_PROCESS_H_

#include <windows.h>
#include <string_view>

DWORD getProcessId(std::string_view processName);
HANDLE getProcessHandleByName(std::string_view processName);
uintptr_t getModuleBaseAddress(DWORD procId, std::wstring_view moduleName);
uintptr_t getModuleBaseAddress(DWORD procId, std::string_view moduleName);

#endif //WHATTHEHACK_UTILITY_INCLUDE_PROCESS_H_
