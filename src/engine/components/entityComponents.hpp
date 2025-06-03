#pragma once

#include "scripts/script.hpp"

#include <entt/entt.hpp>
#include "raylib.h"
#include "raylib-cpp.hpp"

struct lifeComponent {
    float life;
    float maxLife;
};

struct staminaComponent {
    float stamina;
    float maxStamina;
};

struct damageComponent {
    float damage;
    float currentDamage;
    float modifier;
    float multiplier;
};

struct attackComponent {
    float cost;
    float cooldown;
    float currentCooldown;
};

struct abilityComponent {
    std::string name;
    float cost;
    float cooldown;
    float currentCooldown;
};

struct specialAbilityComponent {
    std::string name;
    float cost;
    float cooldown;
    float currentCooldown;
};