//
// Created by user on 10/2/2022.
//

#ifndef WHATTHEHACK_UTILITY_INCLUDE_MEMORY_H_
#define WHATTHEHACK_UTILITY_INCLUDE_MEMORY_H_

#include <windows.h>
#include <string>
#include <vector>

uintptr_t getMultilevelPointerAddress(HANDLE hProcess, uintptr_t baseAddress, const std::vector<uint32_t>& offsets);
uintptr_t getMultilevelPointerAddress(uintptr_t baseAddress, const std::vector<uint32_t>& offsets);

void patchOperation(HANDLE hProcess, uintptr_t address, const char* opcodeString, size_t size);
void patchOperation(uintptr_t address, BYTE* opcodes, size_t size);
void patchOperation(uintptr_t address, const char* opcodeString, size_t size);


void patchNop(HANDLE hProcess, uintptr_t address, size_t size);
void patchNop(uintptr_t address, size_t size);

class Patcher
{
public:
    explicit Patcher(uintptr_t targetAddress);
    Patcher(Patcher&& patcher) noexcept;

    void patch(const char* opcodeString, size_t size);
    void patch(std::string_view nullTerminatedString);
    void patch(size_t nopSize);
    void restore();

    [[nodiscard]] bool isPatched() const noexcept;
    [[nodiscard]] const bool& patched() const noexcept;
    [[nodiscard]] std::string toString() const noexcept;

    ~Patcher();

    Patcher(const Patcher&) = delete;
    Patcher& operator=(const Patcher&) = delete;
    Patcher& operator=(Patcher&&) = delete;

private:
    void backup();

private:
    bool m_patched{};
    BYTE* m_originOps{};
    size_t m_size{};
    uintptr_t m_targetAddress{};
};

class PatcherManager
{
public:
    using value_type = Patcher;
    using container_type = std::vector<value_type>;

public:
    value_type& create(uintptr_t targetAddress) noexcept;

private:
    container_type m_patchers{};
};

#endif //WHATTHEHACK_UTILITY_INCLUDE_MEMORY_H_
