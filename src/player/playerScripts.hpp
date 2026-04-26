#pragma once

#include "defines/components/components.hpp"
#include "defines/events.hpp"
#include "input/inputSystem.hpp"
#include "player/playerStatus.hpp"
#include "skills/skillsDatabase.hpp"

#include <raymath.h>

class playerScripts : public Script {
public:
    static constexpr float kRangedAttackMoveMultiplier = 0.85f;

    void onCreate() override {
        playerStatusScript.bind<playerStatus>(entity, *registry);

        auto& dispatcher = registry->ctx().get<entt::dispatcher>();
        dispatcher.sink<PlayerMoveIntentEvent>().connect<&playerScripts::onMoveIntent>(this);
        dispatcher.sink<PlayerAttackIntentEvent>().connect<&playerScripts::onAttackIntent>(this);
        dispatcher.sink<PlayerAbilityIntentEvent>().connect<&playerScripts::onAbilityIntent>(this);
        dispatcher.sink<TouchDownEvent>().connect<&playerScripts::onTouchDown>(this);
    }

    void onDestroy() override {
        auto& dispatcher = registry->ctx().get<entt::dispatcher>();
        dispatcher.sink<PlayerMoveIntentEvent>().disconnect<&playerScripts::onMoveIntent>(this);
        dispatcher.sink<PlayerAttackIntentEvent>().disconnect<&playerScripts::onAttackIntent>(this);
        dispatcher.sink<PlayerAbilityIntentEvent>().disconnect<&playerScripts::onAbilityIntent>(this);
        dispatcher.sink<TouchDownEvent>().disconnect<&playerScripts::onTouchDown>(this);
    }

    void onUpdate(float dt) override {
        if (auto* gate = registry->try_get<attack_movement_gate>(entity)) {
            gate->windupRemaining -= dt;
            if (gate->windupRemaining < 0.0f) {
                gate->windupRemaining = 0.0f;
            }
        }

        if (auto* loadout = registry->try_get<ability_loadout>(entity)) {
            for (auto& abilityDef : loadout->abilities) {
                abilityDef.tickCooldown(dt);
            }
        }

        playerStatusScript.instance->onUpdate(dt);
    }

    void onDraw() override {
        playerStatusScript.instance->onDraw();
    }

    void onMoveIntent(const PlayerMoveIntentEvent& event) {
        if (event.handled) {
            return;
        }

        auto* movement = registry->try_get<velocity>(entity);
        auto* playerStatusComp = registry->try_get<status>(entity);
        auto* attackComp = registry->try_get<attack>(entity);
        auto* movementGate = registry->try_get<attack_movement_gate>(entity);
        if (!movement || !playerStatusComp || !attackComp) {
            return;
        }

        if (playerStatusComp->isAttacking()) {
            const bool isRangedTarget =
                attackComp->attackRange == AttackRange::RANGED &&
                attackComp->type == AttackType::TARGET;

            const bool isMeleeAoe =
                attackComp->attackRange == AttackRange::MELEE &&
                attackComp->type == AttackType::AOE;

            if (isRangedTarget) {
                movement->dx = event.movement.x * kRangedAttackMoveMultiplier;
                movement->dy = event.movement.y * kRangedAttackMoveMultiplier;
            } else if (isMeleeAoe && movementGate && movementGate->windupRemaining <= 0.0f) {
                movement->dx = event.movement.x;
                movement->dy = event.movement.y;
            } else {
                movement->dx = 0.0f;
                movement->dy = 0.0f;
            }

            event.handled = true;
            return;
        }

        movement->dx = event.movement.x;
        movement->dy = event.movement.y;

        if (Vector2LengthSqr(event.movement) > 0.0f) {
            playerStatusComp->status = StatusType::RUN;
        } else if (playerStatusComp->status == StatusType::RUN) {
            playerStatusComp->status = StatusType::IDLE;
        }

        event.handled = true;
    }

    void onAttackIntent(const PlayerAttackIntentEvent& event) {
        if (event.handled) {
            return;
        }

        registry->emplace_or_replace<attack_intent>(entity);
        event.handled = true;
    }

    void onAbilityIntent(const PlayerAbilityIntentEvent& event) {
        if (event.handled) {
            return;
        }

        auto* loadout = registry->try_get<ability_loadout>(entity);
        auto* targeting = registry->try_get<ability_targeting_state>(entity);
        auto* enduranceComp = registry->try_get<endurance>(entity);
        if (!loadout || !targeting || !enduranceComp) {
            return;
        }

        if (targeting->hasPendingAbility()) {
            return;
        }

        if (event.abilitySlot < 0 || event.abilitySlot >= static_cast<int>(loadout->abilities.size())) {
            return;
        }

        auto& abilityDef = loadout->abilities[event.abilitySlot];
        if (!abilityDef.isReady() || enduranceComp->stamina < abilityDef.cost) {
            return;
        }

        enduranceComp->consume(abilityDef.cost);
        auto* playerStatusComp = registry->try_get<status>(entity);
        if (playerStatusComp) {
            playerStatusComp->status = (event.abilitySlot == 0) ? StatusType::ABILITY1 : StatusType::ABILITY2;
        }
        targeting->pendingAbilityIndex = event.abilitySlot;
        event.handled = true;
    }

    void onTouchDown(const TouchDownEvent& event) {
        if (event.handled) {
            return;
        }

        auto* loadout = registry->try_get<ability_loadout>(entity);
        auto* targeting = registry->try_get<ability_targeting_state>(entity);
        auto* selfTransform = registry->try_get<transform>(entity);
        if (!loadout || !targeting || !selfTransform || !targeting->hasPendingAbility()) {
            return;
        }

        const int abilityIndex = targeting->pendingAbilityIndex;
        if (abilityIndex < 0 || abilityIndex >= static_cast<int>(loadout->abilities.size())) {
            targeting->clear();
            return;
        }

        auto cameraPtr = registry->ctx().find<Camera2D>();
        if (!cameraPtr) {
            return;
        }

        const auto& abilityDef = loadout->abilities[abilityIndex];
        const Vector2 worldPos = GetScreenToWorld2D(event.position, *cameraPtr);

        auto findTargetAtPointer = [&](Vector2 point) -> entt::entity {
            auto candidates = registry->view<transform, collider, health, status>();
            for (auto candidate : candidates) {
                auto& candidateStatus = candidates.get<status>(candidate);
                if (candidateStatus.isDead()) {
                    continue;
                }

                const bool isEnemy = registry->any_of<is_enemy>(candidate);
                if (abilityDef.type == AbilityType::DEBUFFING && !isEnemy) {
                    continue;
                }
                if (abilityDef.type == AbilityType::BUFFING && isEnemy) {
                    continue;
                }

                auto& t = candidates.get<transform>(candidate);
                auto& c = candidates.get<collider>(candidate);
                const Rectangle rect = c.getRect(t.position);
                if (!CheckCollisionPointRec(point, rect)) {
                    continue;
                }

                const Vector2 selfCenter = selfTransform->position;
                const Vector2 candidateCenter = c.getCenter(t.position);
                if (Vector2Distance(selfCenter, candidateCenter) > abilityDef.range) {
                    continue;
                }

                return candidate;
            }

            return entt::null;
        };

        const entt::entity target = findTargetAtPointer(worldPos);
        if (target == entt::null) {
            return;
        }

        const SkillEffectDefinition* effectDef = SkillsDatabase::getStatusEffectDefinition(abilityDef.statusEffectId);
        if (!effectDef) {
            return;
        }

        status_effect_instance incomingEffect;
        incomingEffect.skillId = abilityDef.statusEffectId;
        incomingEffect.source = entity;
        incomingEffect.remainingTime = effectDef->duration;

        auto& effects = registry->get_or_emplace<status_effects>(target);
        effects.addOrRefresh(incomingEffect);

        loadout->abilities[abilityIndex].currentCooldown = loadout->abilities[abilityIndex].cooldown;
        targeting->clear();
        event.handled = true;
    }

    virtual ~playerScripts() = default;

protected:
    script playerStatusScript;
};