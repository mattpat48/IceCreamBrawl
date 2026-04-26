#pragma once

#include "engine/script.hpp"

#include <entt/entt.hpp>
#include <algorithm>
#include <cmath>
#include <memory>
#include <string>
#include <vector>
#include "raylib.h"
#include "raylib-cpp.hpp"

enum class Directions { UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT };
enum class StatusType { IDLE, ATTACK, ABILITY1, ABILITY2, DODGE, RUN, DEAD };

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
    StatusType status;

    bool isIdle() const {
        return status == StatusType::IDLE;
    }
    bool isAttacking() const {
        return status == StatusType::ATTACK;
    }
    bool isUsingAbility() const {
        return status == StatusType::ABILITY1 || status == StatusType::ABILITY2;
    }
    bool isDodging() const {
        return status == StatusType::DODGE;
    }
    bool isDead() const {
        return status == StatusType::DEAD;
    }

    bool isRunning() const {
        return status == StatusType::RUN;
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

// Campo extra sul player per lanciare il proiettile ranged senza metterlo nelle playerTextures.
struct ranged_projectile_emitter {
    std::shared_ptr<raylib::Texture2D> texture;
    float speed = 820.0f;
    float hitRadius = 24.0f;
    float maxLifetime = 2.5f;
};

struct homing_projectile {
    entt::entity attacker = entt::null;
    entt::entity target = entt::null;
    float damage = 0.0f;
    float speed = 820.0f;
    float hitRadius = 24.0f;
    float lifetime = 2.5f;
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

enum class AbilityType {
    BUFFING,
    DEBUFFING,
    DAMAGING
};

struct ability_definition {
    std::string name;
    AbilityType type = AbilityType::BUFFING;
    float cost = 0.0f;
    float cooldown = 0.0f;
    float currentCooldown = 0.0f;
    float range = 0.0f;
    std::string statusEffectId;

    bool isReady() const {
        return currentCooldown <= 0.0f;
    }

    void tickCooldown(float dt) {
        if (currentCooldown <= 0.0f) {
            return;
        }

        currentCooldown -= dt;
        if (currentCooldown < 0.0f) {
            currentCooldown = 0.0f;
        }
    }
};

struct ability_targeting_state {
    int pendingAbilityIndex = -1;

    bool hasPendingAbility() const {
        return pendingAbilityIndex >= 0;
    }

    void clear() {
        pendingAbilityIndex = -1;
    }
};

struct ability_loadout {
    std::vector<ability_definition> abilities;
};

struct status_effect_instance {
    std::string skillId;
    entt::entity source = entt::null;
    float remainingTime = 0.0f;
    float tickAccumulator = 0.0f;
    bool singleActivationDone = false;
};

struct status_effects {
    std::vector<status_effect_instance> active;

    void addOrRefresh(const status_effect_instance& incoming) {
        for (auto& effect : active) {
            if (effect.skillId == incoming.skillId && effect.source == incoming.source) {
                effect.remainingTime = std::max(effect.remainingTime, incoming.remainingTime);
                return;
            }
        }

        active.push_back(incoming);
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

// Governa il lock movimento durante la fase di wind-up degli attacchi.
struct attack_movement_gate {
    float windupRemaining = 0.0f;
    bool lockFacing = false;
    Directions facing = Directions::DOWN;
};

struct target_lock {
    entt::entity target = entt::null;
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

struct is_primary_attack {

};

struct is_hidden {

};

struct is_inactive {

};

struct is_aggroed {
    bool aggroed;
};

enum class WanderState { STILL, MOVING };

struct random_movement {
    WanderState state = WanderState::STILL;
    float timer = 0.0f;

    // Config
    float minIdleTime = 2.0f;
    float maxIdleTime = 5.0f;
    float moveDuration = 0.5f;
    float moveSpeed = 100.0f;

    // State-specific data
    float currentIdleDuration = 2.0f; // The duration for the current idle phase
    Vector2 moveDirection = {0.0f, 0.0f};
};