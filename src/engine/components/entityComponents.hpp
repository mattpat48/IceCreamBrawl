#pragma once

#include "scripts/script.hpp"

#include <entt/entt.hpp>
#include "raylib.h"
#include "raylib-cpp.hpp"

struct status {
    int status;

    bool isIdle() const {
        return status == IDLE;
    }
    bool isAttacking() const {
        return status == ATTACK;
    }
    bool isDodging() const {
        return status == DODGE;
    }
    bool isDead() const {
        return status == DEAD;
    }
};

struct health {
    float life;
    float maxLife;
    float regenRate; // amount of health regenerated per 1/2 second

    void reset() {
        life = maxLife;
    }
    void consume(float amount) {
        life -= amount;
        if (life < 0) {
            life = 0;
        }
    }
};

struct endurance {
    float stamina;
    float maxStamina;
    float regenRate; // amount of stamina regenerated per 1/2 second

    void reset() {
        stamina = maxStamina;
    }
    void consume(float amount) {
        stamina -= amount;
        if (stamina < 0) {
            stamina = 0;
        }
    }
};

struct damage {
    float damage;
    float currentDamage;
    float modifier;
    float multiplier;

    void reset() {
        currentDamage = damage;
    }
    void applyModifier(float mod) {
        modifier = mod;
        currentDamage = damage + modifier;
    }
    void applyMultiplier(float mult) {
        multiplier = mult;
        currentDamage = damage * multiplier;
    }
};

struct attack {
    float cost;
    float cooldown;
    bool modifier;
    float cooldownModifier;
    float currentCooldown;

    void reset() {
        currentCooldown = 0.0f;
    }
    bool isReady() const {
        return currentCooldown <= 0.0f;
    }
    bool canAttack(float stamina) const {
        return isReady() && stamina >= cost;
    }
    void updateCooldown(float delta) {
        if (currentCooldown > 0.0f) {
            currentCooldown -= delta;
            if (currentCooldown < 0.0f) {
                reset();
            }
        }
    }
};

struct ability {
    std::string name;
    float cost;
    float cooldown;
    float currentCooldown;

    void reset() {
        currentCooldown = 0.0f;
    }
    bool isReady() const {
        return currentCooldown <= 0.0f;
    }
    void updateCooldown(float delta) {
        if (currentCooldown > 0.0f) {
            currentCooldown -= delta;
            if (currentCooldown < 0.0f) {
                currentCooldown = 0.0f;
            }
        }
    }
};

struct special {
    std::string name;
    float cost;
    float cooldown;
    float currentCooldown;

    void reset() {
        currentCooldown = 0.0f;
    }
    bool isReady() const {
        return currentCooldown <= 0.0f;
    }
    void updateCooldown(float delta) {
        if (currentCooldown > 0.0f) {
            currentCooldown -= delta;
            if (currentCooldown < 0.0f) {
                currentCooldown = 0.0f;
            }
        }
    }
};