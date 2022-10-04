//
// Created by user on 9/29/2022.
//

#include <Windows.h>
#include <algorithm>
#include <exception>
#include <ios>
#include <iostream>
#include <sstream>
#include "utils.h"
#include "logger.h"

std::string toLower(std::string_view str)
{
    std::string lowercase{};
    lowercase.resize(str.size());
    std::transform(
        std::cbegin(str),
        std::cend(str),
        std::begin(lowercase),
        [](std::uint8_t c)
        { return std::tolower(c); });
    std::cout << __func__ << ": lowercase = " << lowercase << '\n';
    return lowercase;
}

bool isCaseInsensitivelyEqual(std::string_view str1, std::string_view str2)
{
    std::cout << __func__ << ": str1 = " << str1 << '\n';
    std::cout << __func__ << ": str2 = " << str2 << '\n';
    std::string&& lower1{ toLower(str1) };
    std::cout << __func__ << ": lower1 = " << lower1 << '\n';
    std::string&& lower2{ toLower(str2) };
    std::cout << __func__ << ": lower2 = " << lower2 << '\n';
    return lower1 == lower2;
}

DWORD parseProcessId(std::string_view integral)
{
    std::cout << __func__ << ": integral = " << integral << '\n';
    std::stringstream ss{};
    ss << std::hex << integral;
    DWORD processId{};
    ss >> processId;
    std::cout << __func__ << ": processId = " << processId << '\n';
    return processId;
}

std::string byteArrayToHexString(BYTE* bytes, size_t size)
{
    std::stringstream ss{};
    for (size_t i{ 0 }; i < size; ++i)
    {
        ss << "\\x" << std::hex << static_cast<int>(bytes[i]);
    }
    return ss.str();
}

std::filesystem::path parsePath(std::string_view pathString)
{
    std::filesystem::path path{};

    try
    {
        std::istringstream iss{ pathString.data() };
        iss >> path;
    }
    catch (const std::exception& e)
    {
        error(__FILE__, __LINE__, __func__, e.what());
    }

    return path;
}
