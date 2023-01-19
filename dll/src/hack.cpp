//
// Created by user on 10/5/2022.
//
#include "hack.h"
#include <windows.h>
#include "entity.h"
#include "memory.h"
#include "subroutines.h"

void hackMain(uintptr_t moduleBaseAddress) {
    auto getPlayerCrosshairPointingAt{reinterpret_cast<getPlayerWhomCrosshairPointingAt>(moduleBaseAddress + 0x607C0)};

    auto localPlayerEntity{*reinterpret_cast<Player **>(moduleBaseAddress + 0x10f4f4)};

    Patcher recoilPatcher{moduleBaseAddress + 0x63786};

    bool infiniteHealing{};
    bool infiniteAmmo{};
    bool triggerBot{};
    const auto &noRecoil{recoilPatcher.patched()};
    bool isSpectating{};
    bool isInvisible{};

    while (true) {
        if (GetAsyncKeyState(VK_END) & 1) {
            break;
        }

        if (GetAsyncKeyState(VK_NUMPAD1) & 1) {
            infiniteHealing = !infiniteHealing;
        }

        if (GetAsyncKeyState(VK_NUMPAD2) & 1) {
            infiniteAmmo = !infiniteAmmo;
        }

        if (GetAsyncKeyState(VK_NUMPAD3) & 1) {
            triggerBot = !triggerBot;
        }

        if (GetAsyncKeyState(VK_NUMPAD4) & 1) {
            if (noRecoil) {
                recoilPatcher.restore();
            } else {
                recoilPatcher.patch(10);
            }
        }

        if (GetAsyncKeyState(VK_NUMPAD5) & 1) {
            localPlayerEntity->spectateMode = (isSpectating = !isSpectating) ? spectateModeOn : modeOff;
        }

        if (GetAsyncKeyState(VK_NUMPAD6) & 1) {
            localPlayerEntity->invisibleMode = (isInvisible = !isInvisible) ? invisibleModeOn : modeOff;
        }

        if (infiniteHealing) {
            localPlayerEntity->health = 1337;
        }

        if (infiniteAmmo) {
            localPlayerEntity->currentWeapon->mainAmmo->ammo = 1337;
        }

        if (triggerBot) {
            auto crosshairPointedPlayer{getPlayerCrosshairPointingAt()};

            if (crosshairPointedPlayer != nullptr) {
                if (crosshairPointedPlayer->team != localPlayerEntity->team) {
                    localPlayerEntity->isAttacking = 1;
                }
            } else {
                localPlayerEntity->isAttacking = 0;
            }
        }

        Sleep(5);
    }
}