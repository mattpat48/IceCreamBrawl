#pragma once

#include "components/components.hpp"
#include "scripts/entities/entityStatus.hpp"
#include "enemyAttack.hpp"

class enemyScript : public Script {
public:
    enemyScript(entt::entity enemy, entt::registry& registry, entt::entity player) {
        this->player = player;
        this->entity = enemy; // Store the enemy entity reference
        this->registry = &registry; // Store the registry reference
    }

    void onUpdate(float dt) override {
    }

    void onCreate() override {
        enemyStatus.bind<entityStatus>(entity, *registry);
        primaryAttack.bind<enemyAttack>(entity, player, *registry);
    }

    void onDraw() override {
    }

    virtual ~enemyScript() = default;

protected:
    entt::entity player; // Reference to the player entity
    script enemyStatus;
    script primaryAttack;
};