//
// Created by user on 10/5/2022.
//

#ifndef WHATTHEHACK_DLL_INCLUDE_HACK_H_
#define WHATTHEHACK_DLL_INCLUDE_HACK_H_

#include <cstdint>

constexpr int8_t spectateModeOn{ 5 };
constexpr int8_t invisibleModeOn{ 11 };
constexpr int8_t modeOff{ 0 };

void hackMain(uintptr_t moduleBaseAddress);

#endif //WHATTHEHACK_DLL_INCLUDE_HACK_H_
