//
// Created by user on 10/2/2022.
//
#include "memory.h"
#include <ios>
#include <iostream>
#include <sstream>
#include <utility>
#include "utils.h"

uintptr_t getMultilevelPointerAddress(HANDLE hProcess, uintptr_t baseAddress, const std::vector<uint32_t>& offsets)
{
    uintptr_t address{ baseAddress };

    for (auto offset : offsets)
    {
        ReadProcessMemory(hProcess, reinterpret_cast<BYTE*>(address), &address, sizeof(address), nullptr);
        std::cout << __func__ << ": address = " << std::hex << std::showbase << address << '\n';
        address += offset;
    }

    return address;
}

uintptr_t getMultilevelPointerAddress(uintptr_t baseAddress, const std::vector<uint32_t>& offsets)
{
    auto address{ baseAddress };

    for (auto offset : offsets)
    {
        address = *reinterpret_cast<decltype(address)*>(address);
        std::cout << __func__ << ": address = " << std::hex << std::showbase << address << '\n';
        address += offset;
    }

    return address;
}

void patchOperation(HANDLE hProcess, uintptr_t address, const char* opcodeString, size_t size)
{
    DWORD oldProtect{};
    VirtualProtectEx(hProcess,
        reinterpret_cast<BYTE*>(address),
        size,
        PAGE_EXECUTE_READWRITE,
        &oldProtect);
    WriteProcessMemory(hProcess,
        reinterpret_cast<BYTE*>(address),
        reinterpret_cast<BYTE*>(const_cast<char*>(opcodeString)),
        size,
        nullptr);
    VirtualProtectEx(hProcess, reinterpret_cast<BYTE*>(address), size, oldProtect, &oldProtect);
}

void patchOperation(uintptr_t address, const char* opcodeString, size_t size)
{
    DWORD oldProtect{};
    std::cout << "sizeof(opcodeString) = " << size << '\n';
    VirtualProtect(reinterpret_cast<BYTE*>(address), size, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(reinterpret_cast<BYTE*>(address),
        reinterpret_cast<BYTE*>(const_cast<char*>(opcodeString)),
        size);
    VirtualProtect(reinterpret_cast<BYTE*>(address), size, oldProtect, &oldProtect);
}

void patchOperation(uintptr_t address, BYTE* opcodes, size_t size)
{
    DWORD oldProtect{};
    VirtualProtect(reinterpret_cast<BYTE*>(address), size, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(reinterpret_cast<BYTE*>(address), opcodes, size);
    VirtualProtect(reinterpret_cast<BYTE*>(address), size, oldProtect, &oldProtect);
}

void patchNop(HANDLE hProcess, uintptr_t address, size_t size)
{
    patchOperation(hProcess, address, std::string(size, '\x90').data(), size);
}

void patchNop(uintptr_t address, size_t size)
{
    patchOperation(address, std::string(size, '\x90').data(), size);
}

Patcher::Patcher(uintptr_t targetAddress)
    : m_targetAddress{ targetAddress },
      m_patched{ false },
      m_originOps{ nullptr },
      m_size{ 0 }
{
    log(__func__);
}

void Patcher::backup()
{
    if (m_originOps == nullptr)
    {
        m_originOps = new BYTE[m_size];
        memcpy(m_originOps, reinterpret_cast<BYTE*>(m_targetAddress), m_size);
    }
    log(__func__);
}

void Patcher::patch(const char* opcodeString, size_t size)
{
    if (!m_patched)
    {
        m_size = sizeof(opcodeString);
        backup();
        patchOperation(m_targetAddress, opcodeString, m_size);
        m_patched = true;
    }
    log(__func__);
}

void Patcher::patch(std::string_view nullTerminatedString)
{
    patch(nullTerminatedString.data(), nullTerminatedString.size());
}

void Patcher::patch(size_t nopSize)
{
    if (!m_patched)
    {
        m_size = nopSize;
        backup();
        patchNop(m_targetAddress, nopSize);
        m_patched = true;
    }
    log(__func__);
}

void Patcher::restore()
{
    if (m_patched)
    {
        patchOperation(m_targetAddress, m_originOps, m_size);
        m_patched = false;
    }
    log(__func__);
}

Patcher::~Patcher()
{
    if (m_patched)
    {
        restore();
    }

    delete[] m_originOps;
}

bool Patcher::isPatched() const noexcept
{
    return m_patched;
}

const bool& Patcher::patched() const noexcept
{
    return m_patched;
}

std::string Patcher::toString() const noexcept
{
    std::stringstream ss{};
    ss << "m_targetAddress = " << std::hex << std::showbase << m_targetAddress << '\n';
    ss << "m_patched = " << std::boolalpha << m_patched << '\n';
    if (m_patched)
    {
        ss << "m_size = " << std::dec << m_size << '\n';
        ss << "m_originOps = " << byteArrayToHexString(m_originOps, m_size) << '\n';
    }
    return ss.str();
}

void Patcher::log(const char* funcName) const
{
    std::cout << funcName << ":\n"
              << toString() << '\n';
}

Patcher::Patcher(Patcher&& patcher) noexcept: m_patched{ patcher.m_patched },
                                              m_size{ patcher.m_size },
                                              m_targetAddress{ patcher.m_targetAddress },
                                              m_originOps{ patcher.m_originOps }
{
    patcher.m_originOps = nullptr;
}

PatcherManager::value_type& PatcherManager::create(uintptr_t targetAddress) noexcept
{
    return m_patchers.emplace_back(targetAddress);
}
