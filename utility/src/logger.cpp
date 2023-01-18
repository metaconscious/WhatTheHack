//
// Created by user on 10/4/2022.
//
#include "logger.h"
#include <system_error>

std::string LogLevel::name(LogLevel::Level level)
{
    switch (level)
    {
    case Level::INFO_LEVEL:
        return "INFO";
    case Level::DEBUG_LEVEL:
        return "DEBUG";
    case Level::WARN_LEVEL:
        return "WARN";
    case Level::ERROR_LEVEL:
        return "ERROR";
    default:
        return "???";
    }
}

using Level = LogLevel::Level;

void setLogBase(std::ostream& os,
    LogLevel::Level level,
    const std::filesystem::path& fileName,
    size_t lineNumber,
    std::string_view date,
    std::string_view time,
    std::string_view functionName)
{
    os << '[' << date << ' ' << time << ']'
       << '[' << LogLevel::name(level) << ']'
       << '[' << fileName.filename().string() << ':' << lineNumber << ']'
       << functionName << "(): ";
}

void log(std::ostream& os,
    Level level,
    std::string_view fileName,
    size_t lineNumber,
    std::string_view date,
    std::string_view time,
    std::string_view functionName,
    std::string_view message)
{
    setLogBase(os, level, fileName, lineNumber, date, time, functionName);
    os << message << '\n';
}
void log(Level level,
    std::string_view fileName,
    size_t lineNumber,
    std::string_view date,
    std::string_view time,
    std::string_view functionName,
    std::string_view message)
{
    switch (level)
    {
    case Level::INFO_LEVEL:
    case Level::DEBUG_LEVEL:
    case Level::WARN_LEVEL:
        log(std::cout, level, fileName, lineNumber, date, time, functionName, message);
        break;
    case Level::ERROR_LEVEL:
        log(std::cerr, level, fileName, lineNumber, date, time, functionName, message);
        break;
    default:
        log(std::cout, level, fileName, lineNumber, date, time, functionName, message);
    }
}
