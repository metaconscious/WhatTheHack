//
// Created by user on 10/5/2022.
//

#ifndef WHATTHEHACK_GAME_INCLUDE_ENTITY_H_
#define WHATTHEHACK_GAME_INCLUDE_ENTITY_H_

#include <cstdint>
#include "autopad.h"
#include "base.h"

class AmmoClip;
class Weapon;

class Player
{
public:
    union
    {
        DEFINE_MEMBER_N(Vector3, headPosition, 0x4);
        DEFINE_MEMBER_N(Vector3, bodyPosition, 0x34);
        DEFINE_MEMBER_N(Vector3, viewingAngles, 0x40);
        DEFINE_MEMBER_N(int8_t, invisibleMode, 0x82);
        DEFINE_MEMBER_N(int32_t, health, 0xf8);
        DEFINE_MEMBER_N(int8_t, isAttacking, 0x224);
        DEFINE_MEMBER_N(int32_t, team, 0x32c);
        DEFINE_MEMBER_N(int8_t, spectateMode, 0x338);
        DEFINE_MEMBER_N(Weapon*, currentWeapon, 0x374);
    };
};

class Weapon
{
public:
    union
    {
        DEFINE_MEMBER_N(int32_t, weaponId, 0x4);
        DEFINE_MEMBER_N(Player*, owner, 0x8);
        DEFINE_MEMBER_N(AmmoClip*, backAmmo, 0x10);
        DEFINE_MEMBER_N(AmmoClip*, mainAmmo, 0x14);
    };
};

class AmmoClip
{
public:
    int32_t ammo;
};

#endif //WHATTHEHACK_GAME_INCLUDE_ENTITY_H_
