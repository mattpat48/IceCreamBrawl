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
        auto a = registry->try_get<animation>(playerEntity);
        auto s = registry->try_get<sprite>(playerEntity);
        auto st = registry->try_get<status>(playerEntity);
        auto en = registry->try_get<endurance>(playerEntity);
        auto atk = registry->try_get<attack>(playerEntity);

        if (a && s && st && en && atk) {
            std::cout << "Primary button released!" << std::endl;
            if (st->isIdle() && atk->canAttack(en->stamina)) {
                std::cout << "Primary button released, performing attack!" << std::endl;
                en->consume(atk->cost); // Consume stamina for attack
                s->currentTexture = "attack"; // Change to attack texture on button release
                a->currentFrame = a->startFrame; // Reset animation frame to start frame
                st->status = ATTACK;
            }
        }
    }

protected:
    entt::entity playerEntity; // Reference to the player entity
    entt::entity enemyEntity; // Reference to the enemy entity (if needed)
};