//
// Created by user on 10/4/2022.
//

#ifndef WHATTHEHACK_UTILITY_INCLUDE_LOGGER_H_
#define WHATTHEHACK_UTILITY_INCLUDE_LOGGER_H_

#include <string_view>

void error(std::string_view fileName, size_t lineNumber, std::string_view functionName, std::string_view message);
void printErrorMessageIfOccurred(std::string_view fileName, size_t lineNumber, std::string_view functionName);

#endif //WHATTHEHACK_UTILITY_INCLUDE_LOGGER_H_
