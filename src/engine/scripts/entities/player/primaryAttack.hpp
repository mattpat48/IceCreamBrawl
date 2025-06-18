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
        auto ani = registry->try_get<animation>(playerEntity);
        auto spr = registry->try_get<sprite>(playerEntity);
        auto sta = registry->try_get<status>(playerEntity);
        auto end = registry->try_get<endurance>(playerEntity);
        auto atk = registry->try_get<attack>(playerEntity);
        auto dmg = registry->try_get<damage>(playerEntity);

        auto esta = registry->try_get<status>(enemyEntity);
        auto ehea = registry->try_get<health>(enemyEntity);
        auto eani = registry->try_get<animation>(enemyEntity);
        auto espr = registry->try_get<sprite>(enemyEntity);
        auto ehit = registry->try_get<hitFlash>(enemyEntity);

        if (ani && spr && sta && end && atk && esta && ehea && espr) {
            if (sta->isIdle() && atk->canAttack(end->stamina) && !esta->isDead()) {
                end->consume(atk->cost); // Consume stamina for attack
                spr->currentTexture = "attack"; // Change to attack texture on button release
                ani->currentFrame = ani->startFrame; // Reset animation frame to start frame
                sta->status = ATTACK;

                if (!esta->isDodging()) {
                    ehea->consume(dmg->currentDamage); // Apply damage to enemy if not dodging
                    ehit->filter = RED;
                    ehit->timeFlash = 0.0f;
                    std::cout << "Enemy took damage: " << dmg->currentDamage << std::endl;
                } else {
                    std::cout << "Enemy is dodging, no damage taken." << std::endl;
                }
            }
        }
    }

protected:
    entt::entity playerEntity; // Reference to the player entity
    entt::entity enemyEntity; // Reference to the enemy entity (if needed)
};