#pragma once

#include <string>

#include <entt/entt.hpp>

#include "defines/components/components.hpp"

struct SkillEffectContext {
    entt::registry& registry;
    entt::entity source = entt::null;
    entt::entity target = entt::null;
    float deltaTime = 0.0f;
};

enum class StatusEffectActivationMode {
    SINGLE_ACTIVATION,
    CONTINUOUS_ACTIVATION
};

using SkillEffectHandler = void (*)(SkillEffectContext& context);

struct SkillEffectDefinition {
    std::string id;
    std::string shortLabel;
    Color iconColor = WHITE;
    StatusEffectActivationMode activationMode = StatusEffectActivationMode::SINGLE_ACTIVATION;
    float duration = 0.0f;
    float tickInterval = 0.0f;
    SkillEffectHandler onApply = nullptr;
    SkillEffectHandler onTick = nullptr;
    SkillEffectHandler onExpire = nullptr;
};

class SkillsDatabase {
public:
    static const SkillEffectDefinition* getStatusEffectDefinition(const std::string& id) {
        if (id == "cripple_attack") {
            static const SkillEffectDefinition def{
                "cripple_attack",
                "CRP",
                ORANGE,
                StatusEffectActivationMode::SINGLE_ACTIVATION,
                6.0f,
                0.0f,
                &applyCrippleAttack,
                nullptr,
                &expireCrippleAttack
            };
            return &def;
        }

        if (id == "fortify_health") {
            static const SkillEffectDefinition def{
                "fortify_health",
                "FRT",
                GREEN,
                StatusEffectActivationMode::SINGLE_ACTIVATION,
                6.0f,
                0.0f,
                &applyFortifyHealth,
                nullptr,
                &expireFortifyHealth
            };
            return &def;
        }

        if (id == "burning_dot") {
            static const SkillEffectDefinition def{
                "burning_dot",
                "DOT",
                RED,
                StatusEffectActivationMode::CONTINUOUS_ACTIVATION,
                6.0f,
                1.0f,
                nullptr,
                &tickBurningDamage,
                nullptr
            };
            return &def;
        }

        return nullptr;
    }

private:
    static void applyCrippleAttack(SkillEffectContext& context) {
        auto* dmg = context.registry.try_get<damage>(context.target);
        if (!dmg) {
            return;
        }

        dmg->multiplier *= 0.8f;
        dmg->currentDamage = (dmg->damage + dmg->modifier) * dmg->multiplier;
    }

    static void expireCrippleAttack(SkillEffectContext& context) {
        auto* dmg = context.registry.try_get<damage>(context.target);
        if (!dmg) {
            return;
        }

        dmg->multiplier /= 0.8f;
        if (dmg->multiplier < 0.0f) {
            dmg->multiplier = 0.0f;
        }
        dmg->currentDamage = (dmg->damage + dmg->modifier) * dmg->multiplier;
    }

    static void applyFortifyHealth(SkillEffectContext& context) {
        auto* hp = context.registry.try_get<health>(context.target);
        if (!hp) {
            return;
        }

        hp->maxLife *= 1.1f;
        hp->life *= 1.1f;
        if (hp->life > hp->maxLife) {
            hp->life = hp->maxLife;
        }
    }

    static void expireFortifyHealth(SkillEffectContext& context) {
        auto* hp = context.registry.try_get<health>(context.target);
        if (!hp) {
            return;
        }

        hp->maxLife /= 1.1f;
        if (hp->maxLife < 1.0f) {
            hp->maxLife = 1.0f;
        }
        if (hp->life > hp->maxLife) {
            hp->life = hp->maxLife;
        }
    }

    static void tickBurningDamage(SkillEffectContext& context) {
        auto* hp = context.registry.try_get<health>(context.target);
        if (!hp) {
            return;
        }

        hp->consume(1.0f);
    }
};
