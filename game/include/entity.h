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

class PlayerEntity
{
public:
    union
    {
        DEFINE_MEMBER_N(int32_t, health, 0xf8);
        DEFINE_MEMBER_N(Weapon*, currentWeapon, 0x374);
    };
};

class Weapon
{
public:
    union
    {
        DEFINE_MEMBER_N(AmmoClip*, ammoClip, 0x14);
    };
};

class AmmoClip
{
public:
    int32_t ammo;
};

#endif //WHATTHEHACK_GAME_INCLUDE_ENTITY_H_
