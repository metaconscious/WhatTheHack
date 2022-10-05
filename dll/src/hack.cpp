//
// Created by user on 10/5/2022.
//
#include "hack.h"
#include <windows.h>
#include "entity.h"
#include "memory.h"

void hackMain(uintptr_t moduleBaseAddress)
{
    auto localPlayerEntity{ *reinterpret_cast<PlayerEntity**>(moduleBaseAddress + 0x10f4f4) };

    Patcher recoilPatcher{ moduleBaseAddress + 0x63786 };

    bool infiniteHealing{};
    bool infiniteAmmo{};
    const auto& noRecoil{ recoilPatcher.patched() };

    while (true)
    {
        if (GetAsyncKeyState(VK_END) & 1)
        {
            break;
        }

        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            infiniteHealing = !infiniteHealing;
        }

        if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        {
            infiniteAmmo = !infiniteAmmo;
        }

        if (GetAsyncKeyState(VK_NUMPAD3) & 1)
        {
            if (noRecoil)
            {
                recoilPatcher.restore();
            }
            else
            {
                recoilPatcher.patch(10);
            }
        }

        if (infiniteHealing)
        {
            localPlayerEntity->health = 1337;
        }

        if (infiniteAmmo)
        {
            localPlayerEntity->currentWeapon->ammoClip->ammo = 1337;
        }

        Sleep(5);
    }
}