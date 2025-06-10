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
        enemyStatus.instance->onUpdate(dt);
        auto stat = getComponent<status>();
        if (stat->isDead()) {
            std::cout << "Enemy is dead!" << std::endl;
            return;
        }
        primaryAttack.instance->onUpdate(dt);
    }

    void onCreate() override {
        enemyStatus.bind<entityStatus>(entity, *registry);
        primaryAttack.bind<enemyAttack>(entity, player, *registry);
    }

    void onDraw() override {
        auto pl = getComponent<health>();
        DrawText(("ENEMY HP: " + std::to_string(static_cast<int>(pl->life)) + "/" + std::to_string(static_cast<int>(pl->maxLife))).c_str(), 10, 90, 20, RED);
        auto ps = getComponent<endurance>();
        DrawText(("ENEMY ST: " + std::to_string(static_cast<int>(ps->stamina)) + "/" + std::to_string(static_cast<int>(ps->maxStamina))).c_str(), 10, 110, 20, BLUE);
    }

protected:
    entt::entity player; // Reference to the player entity
    script enemyStatus;
    script primaryAttack;
};