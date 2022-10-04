//
// Created by user on 10/4/2022.
//
#include "logger.h"
#include <windows.h>
#include <system_error>
#include <iostream>

void error(std::string_view fileName, size_t lineNumber, std::string_view functionName, std::string_view message)
{
    std::cerr << '[' << fileName << ':' << lineNumber << "] "
              << functionName << ": " << message << '\n';
}

void printErrorMessageIfOccurred(std::string_view fileName, size_t lineNumber, std::string_view functionName)
{
    auto errorCode{ GetLastError() };

    if (errorCode == 0)
    {
        return;
    }

    error(fileName, lineNumber, fileName, std::system_category().message(static_cast<int>(errorCode)));

    exit(static_cast<int>(errorCode));
}