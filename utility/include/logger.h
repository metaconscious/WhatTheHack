//
// Created by user on 10/4/2022.
//

#ifndef WHATTHEHACK_UTILITY_INCLUDE_LOGGER_H_
#define WHATTHEHACK_UTILITY_INCLUDE_LOGGER_H_

#include <concepts>
#include <iostream>
#include <string>
#include <string_view>
#include <system_error>

#ifdef _DEBUG

#define Debug(MESSAGE) log(LogLevel::Level::DEBUG_LEVEL, __FILE__, __LINE__, __DATE__, __TIME__, __func__, MESSAGE)
#define DebugVarInfo(VARIABLE) debug(LogLevel::Level::DEBUG_LEVEL, __FILE__, __LINE__, __DATE__, __TIME__, __func__, #VARIABLE, VARIABLE)
#define DebugVarInfoFmt(VARIABLE, ...) debug(LogLevel::Level::DEBUG_LEVEL, __FILE__, __LINE__, __DATE__, __TIME__, __func__, #VARIABLE, VARIABLE, __VA_ARGS__)

#else

#define Debug(MESSAGE)
#define DebugVarInfo(VARIABLE)
#define DebugVarInfoFmt(VARIABLE, ...)

#endif

#define Info(MESSAGE) log(LogLevel::Level::INFO_LEVEL, __FILE__, __LINE__, __DATE__, __TIME__, __func__, MESSAGE)
#define Warn(MESSAGE) log(LogLevel::Level::WARN_LEVEL, __FILE__, __LINE__, __DATE__, __TIME__, __func__, MESSAGE)
#define Error(MESSAGE) log(LogLevel::Level::ERROR_LEVEL, __FILE__, __LINE__, __DATE__, __TIME__, __func__, MESSAGE)
#define SystemError(ERROR_CODE) Error(std::system_category().message(static_cast<int>(ERROR_CODE)))

class LogLevel
{
public:
    enum class Level
    {
        INFO_LEVEL,
        DEBUG_LEVEL,
        WARN_LEVEL,
        ERROR_LEVEL
    };

    static std::string name(Level level);
};

void setLogBase(std::ostream& os,
    LogLevel::Level level,
    std::string_view fileName,
    size_t lineNumber,
    std::string_view date,
    std::string_view time,
    std::string_view functionName);

void log(LogLevel::Level level,
    std::string_view fileName,
    size_t lineNumber,
    std::string_view date,
    std::string_view time,
    std::string_view functionName,
    std::string_view message);

template<typename T>
void debug(LogLevel::Level level,
    std::string_view fileName,
    size_t lineNumber,
    std::string_view date,
    std::string_view time,
    std::string_view functionName,
    std::string_view variableName,
    T value)
{
    logDebugVariableInfo(std::cout, level, fileName, lineNumber, date, time, functionName, variableName, value);
}

template<typename T>
void logDebugVariableInfo(std::ostream& os,
    LogLevel::Level level,
    std::string_view fileName,
    size_t lineNumber,
    std::string_view date,
    std::string_view time,
    std::string_view functionName,
    std::string_view variableName,
    T value)
{
    setLogBase(os, level, fileName, lineNumber, date, time, functionName);
    os << variableName << " = " << value << '\n';
}

template<typename T, typename ...FmtFlags>
void debug(LogLevel::Level level,
    std::string_view fileName,
    size_t lineNumber,
    std::string_view date,
    std::string_view time,
    std::string_view functionName,
    std::string_view variableName,
    T value,
    FmtFlags... flags)
{
    logDebugVariableInfoFmt(std::cout,
        level,
        fileName,
        lineNumber,
        date,
        time,
        functionName,
        variableName,
        value,
        std::forward<FmtFlags>(flags)...);
}

template<typename T, typename ...FmtFlags>
void logDebugVariableInfoFmt(std::ostream& os,
    LogLevel::Level level,
    std::string_view fileName,
    size_t lineNumber,
    std::string_view date,
    std::string_view time,
    std::string_view functionName,
    std::string_view variableName,
    T value,
    FmtFlags... flags)
{
    setLogBase(os, level, fileName, lineNumber, date, time, functionName);
    os << variableName << " = ";
    auto oldMask{ os.flags((flags | ...)) };
    os << value;
    os.flags(oldMask);
    os << '\n';
}

#endif //WHATTHEHACK_UTILITY_INCLUDE_LOGGER_H_
