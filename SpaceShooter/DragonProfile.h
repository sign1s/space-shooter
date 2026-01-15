#pragma once

#include <string>

struct DragonProfile {
    std::string name;
    std::string texturePath;
    int hpMax;
    int damage;
    int shootTimerMax;
    int price;
    bool isUnlocked;
};