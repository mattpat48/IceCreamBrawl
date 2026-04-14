#pragma once

#include "engine/script.hpp"
#include "defines/defines_general.h"

#include <entt/entt.hpp>
#include "raylib.h"
#include "raylib-cpp.hpp"

struct hit_flash {
    Color filter = WHITE;
    float timeFlash = 0.0f;
};

struct collider {
    float offsetX;
    float offsetY;
    float width;
    float height;

    Rectangle getRect(Vector2 position) const {
        return { position.x + offsetX, position.y + offsetY, width, height };
    }
    
    Vector2 getCenter(Vector2 position) const {
        return { position.x + offsetX + width / 2.0f, position.y + offsetY + height / 2.0f };
    }
};

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

    bool isRunning() const {
        return status == RUN;
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

    void regen(float amount) {
        life += amount;
        if (life > maxLife) {
            life = maxLife;
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

    void regen(float amount) {
        stamina += amount;
        if (stamina > maxStamina) {
            stamina = maxStamina;
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

enum class AttackRange { MELEE, RANGED };
enum class AttackType { TARGET, AOE };
enum class AttackShape { NONE, CIRCLE, CONE, LINE }; // NONE è usato per i TARGET

struct attack {
    float cost;
    float cooldown;
    float range;
    
    // Nuovi parametri architetturali
    AttackRange attackRange;
    AttackType type;
    AttackShape shape;
    float angle; // Usato per il CONE (in gradi, es. 120.0f)

    float costModifier;
    float cooldownModifier;
    float rangeModifier;
    
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

// Componente temporaneo per mostrare l'area o il target dell'attacco
struct attack_feedback {
    float lifetime;
    float maxLifetime;
    AttackType type;
    AttackShape shape;
    float angle;
    float range;
    Vector2 origin;
    Vector2 direction;
    entt::entity target; // Usato solo se type == TARGET
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

struct attack_intent {

};

struct damage_received {
    float amount;
};

struct death_event {
    bool isPlayer;
};

struct is_enemy {

};

struct is_player {

};

struct is_minimap {

};

struct is_joystick {

};

struct is_aggroed {
    bool aggroed;
};

struct random_movement {
    float changeDirectionTime;
    float timeSinceLastChange;
    Vector2 direction;
};