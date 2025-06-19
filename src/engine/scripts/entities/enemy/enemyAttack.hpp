#pragma once

#include "components/components.hpp"

#include <iostream>

class enemyAttack : public Script {
public:
    enemyAttack(entt::entity entity, entt::entity player, entt::registry& registry) {
        this->entity = entity;
        this->playerEntity = player;
        this->registry = &registry;
    }

    void onUpdate(float dt) override {
        auto atk = getComponent<attack>();
        auto dmg = getComponent<damage>();
        auto sta = getComponent<status>();
        auto end = getComponent<endurance>();
        auto spr = getComponent<sprite>();
        auto ani = getComponent<animation>();

        auto psta = registry->try_get<status>(playerEntity);
        auto phea = registry->try_get<health>(playerEntity);

        if (atk && dmg && sta && end && spr && ani && psta && phea) {
            if (sta->isIdle() && atk->canAttack(end->stamina) && !psta->isDead()) {// && spr->currentTexture != "hurt") {
                sta->status = ATTACK;
                std::cout << "Enemy Attacking!" << std::endl;

                end->consume(atk->cost); // Consume stamina for attack
                spr->currentTexture = "attack"; // Change to attack texture on button release
                ani->currentFrame = ani->startFrame; // Reset animation frame to start frame

                if (!psta->isDodging()) {
                    phea->consume(dmg->currentDamage); // Apply damage to enemy if not dodging
                    std::cout << "Player took damage: " << dmg->currentDamage << std::endl;
                } else {
                    std::cout << "Enemy is dodging, no damage taken." << std::endl;
                }
            }
        }
    }

    void onDraw() override {

    }

protected:
    entt::entity playerEntity;
};