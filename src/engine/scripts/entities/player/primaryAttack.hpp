#pragma once

#include "components/components.hpp"
#include "scripts/script.hpp"
#include "scripts/general/buttons.hpp"

class primaryAttack : public buttonHandler {
public:
    primaryAttack(entt::entity playerEntity, entt::entity enemyEntity, entt::registry& registry)
        : buttonHandler(registry) {
        this->playerEntity = playerEntity; // Store the player entity for reference
        this->enemyEntity = enemyEntity; // Store the enemy entity for reference
    }

    void buttonRelease() override {
    }

    virtual ~primaryAttack() = default;

protected:
    entt::entity playerEntity; // Reference to the player entity
    entt::entity enemyEntity; // Reference to the enemy entity (if needed)
};