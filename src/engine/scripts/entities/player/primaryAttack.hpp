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
        auto dmg = registry->try_get<damage>(playerEntity);

        auto es = registry->try_get<status>(enemyEntity);
        auto eh = registry->try_get<health>(enemyEntity);
        auto ea = registry->try_get<animation>(enemyEntity);
        auto enSprite = registry->try_get<sprite>(enemyEntity);

        if (a && s && st && en && atk && es && eh && enSprite) {
            if (st->isIdle() && atk->canAttack(en->stamina) && !es->isDead()) {
                en->consume(atk->cost); // Consume stamina for attack
                s->currentTexture = "attack"; // Change to attack texture on button release
                a->currentFrame = a->startFrame; // Reset animation frame to start frame
                st->status = ATTACK;

                if (!es->isDodging()) {
                    eh->consume(dmg->currentDamage); // Apply damage to enemy if not dodging
                    std::cout << "Enemy took damage: " << dmg->currentDamage << std::endl;
                    if (!es->isAttacking()) {
                        enSprite->currentTexture = "hurt"; // Change enemy texture to hurt if not dodging
                        ea->currentFrame = ea->startFrame; // Reset enemy animation frame to start frame
                    }

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