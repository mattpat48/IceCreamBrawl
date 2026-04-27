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

struct SkillHitContext {
    entt::registry& registry;
    entt::entity source = entt::null;
    entt::entity target = entt::null;
    float damageAmount = 0.0f;
};

enum class StatusEffectActivationMode {
    SINGLE_ACTIVATION,
    CONTINUOUS_ACTIVATION
};

using SkillEffectHandler = void (*)(SkillEffectContext& context);
using SkillHitHandler = void (*)(SkillHitContext& context);

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

struct SkillAttackDefinition {
    std::string id;
    SkillCastKind kind = SkillCastKind::MELEE_AOE;
    AttackShape shape = AttackShape::CIRCLE;
    float angle = 0.0f;
    float range = 0.0f;
    float damageMultiplier = 1.0f;
    float frameTime = 0.1f;
    int frameCount = 5;
    SkillHitHandler onHit = nullptr;
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

    static const SkillAttackDefinition* getSkillAttackDefinition(const std::string& id) {
        if (id == "cripple_attack") {
            static const SkillAttackDefinition def{
                "cripple_attack",
                SkillCastKind::RANGED_TARGET,
                AttackShape::NONE,
                0.0f,
                260.0f,
                0.0f,
                0.1f,
                5,
                &onHitCrippleAttack
            };
            return &def;
        }

        if (id == "fortify_health") {
            static const SkillAttackDefinition def{
                "fortify_health",
                SkillCastKind::RANGED_TARGET,
                AttackShape::NONE,
                0.0f,
                260.0f,
                0.0f,
                0.1f,
                5,
                &onHitFortifyHealth
            };
            return &def;
        }

        if (id == "shockwave_strike") {
            static const SkillAttackDefinition def{
                "shockwave_strike",
                SkillCastKind::MELEE_AOE,
                AttackShape::CIRCLE,
                0.0f,
                230.0f,
                1.3f,
                0.1f,
                5,
                &onHitShockwaveStrike
            };
            return &def;
        }

        return nullptr;
    }

private:
    static void applyStatusEffectOnTarget(entt::registry& registry, entt::entity source, entt::entity target, const std::string& effectId) {
        const SkillEffectDefinition* definition = getStatusEffectDefinition(effectId);
        if (!definition) {
            return;
        }

        status_effect_instance instance;
        instance.skillId = effectId;
        instance.source = source;
        instance.remainingTime = definition->duration;

        auto& effects = registry.get_or_emplace<status_effects>(target);
        effects.addOrRefresh(instance);
    }

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

    static void onHitShockwaveStrike(SkillHitContext& context) {
        applyStatusEffectOnTarget(context.registry, context.source, context.target, "burning_dot");
    }

    static void onHitCrippleAttack(SkillHitContext& context) {
        applyStatusEffectOnTarget(context.registry, context.source, context.target, "cripple_attack");
    }

    static void onHitFortifyHealth(SkillHitContext& context) {
        applyStatusEffectOnTarget(context.registry, context.source, context.target, "fortify_health");
    }
};
