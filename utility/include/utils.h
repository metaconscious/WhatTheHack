//
// Created by user on 9/29/2022.
//

#ifndef WHATTHEHACK_APP_INCLUDE_UTILS_H_
#define WHATTHEHACK_APP_INCLUDE_UTILS_H_

#include <windows.h>
#include <string>

DWORD parseProcessId(std::string_view integral);
void printErrorMessageIfOccurred(std::string_view fileName, std::size_t lineNumber, std::string_view functionName);
std::string toLower(std::string_view str);
bool isCaseInsensitivelyEqual(std::string_view str1, std::string_view str2);
std::string byteArrayToHexString(BYTE* bytes, size_t size);

#endif //WHATTHEHACK_APP_INCLUDE_UTILS_H_
