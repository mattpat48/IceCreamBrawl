#pragma once

#include "components/components.hpp"
#include "scripts/general/controller.hpp"
#include "scripts/general/buttons.hpp"
#include "scripts/entities/entityStatus.hpp"
#include <entt/entt.hpp>

#include "raylib.h"
#include "raylib-cpp.hpp"

#include <iostream>

class playerScript : public Script {
public:

    playerScript(entt::entity player, entt::registry& registry) {
        this->entity = player; // Store the player entity reference
        this->registry = &registry; // Store the registry reference
    }

    void onUpdate(float dt) override {
        playerStatus.instance->onUpdate(dt);
        auto stat = getComponent<status>();
        if (stat->isDead()) {
            std::cout << "Player is dead!" << std::endl;
            return; // Skip further updates if the player is dead
        }
        //dirController.instance->onUpdate(dt);
    }

    void onCreate() override {
        dirController.bind<changeDirectionController>(entity, *registry);
        playerStatus.bind<entityStatus>(entity, *registry);
    }

    void onDraw() override {
        auto pl = getComponent<health>();
        DrawText(("PLAYER HP: " + std::to_string(static_cast<int>(pl->life)) + "/" + std::to_string(static_cast<int>(pl->maxLife))).c_str(), 10, 50, 20, RED);
        auto ps = getComponent<endurance>();
        DrawText(("PLAYER ST: " + std::to_string(static_cast<int>(ps->stamina)) + "/" + std::to_string(static_cast<int>(ps->maxStamina))).c_str(), 10, 70, 20, BLUE);
    }

protected:
    entt::entity enemy;
    script dirController;
    script playerStatus;
};