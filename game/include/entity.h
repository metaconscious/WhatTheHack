//
// Created by user on 10/5/2022.
//

#ifndef WHATTHEHACK_GAME_INCLUDE_ENTITY_H_
#define WHATTHEHACK_GAME_INCLUDE_ENTITY_H_

#include <cstdint>
#include "base.h"

class PlayerEntity
{
public:
    Vector3 headPosition; //0x0004
    char pad_0010[36]; //0x0010
    Vector3 bodyPosition; //0x0034
    Vector3 viewingAngle; //0x0040
    char pad_004C[172]; //0x004C
    int32_t health; //0x00F8
    char pad_00FC[297]; //0x00FC
    char name[16]; //0x0225
    char pad_0235[319]; //0x0235
    class Weapon *currentWeapon; //0x0374

    virtual void Function0();
    virtual void Function1();
    virtual void Function2();
    virtual void Function3();
    virtual void Function4();
    virtual void Function5();
    virtual void Function6();
    virtual void Function7();
    virtual void Function8();
    virtual void Function9();
}; //Size: 0x0378
static_assert(sizeof(PlayerEntity) == 0x378);

class Weapon
{
public:
    char pad_0000[20]; //0x0000
    class AmmoClip *ammoClip; //0x0014
}; //Size: 0x0018
static_assert(sizeof(Weapon) == 0x18);

class AmmoClip
{
public:
    int32_t ammo; //0x0000
}; //Size: 0x0004
static_assert(sizeof(AmmoClip) == 0x4);

#endif //WHATTHEHACK_GAME_INCLUDE_ENTITY_H_
