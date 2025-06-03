#pragma once

#include "components/components.hpp"
#include "scripts/script.hpp"
#include "scripts/general/controller.hpp"
#include "scripts/general/buttons.hpp"
#include <entt/entt.hpp>

#include "raylib.h"
#include "raylib-cpp.hpp"

#include <iostream>

class playerScript : public Script {
public:

    playerScript(entt::registry& registry) {
        this->registry = &registry; // Store the registry reference
    }

    void onUpdate(float dt) override {
        dirController.instance->onUpdate(dt);

        auto ps = getComponent<sprite>();
        auto pa = getComponent<animation>();

        if (ps->currentTexture == "attack" && pa->currentFrame == pa->endFrame) {
            std::cout << "Player is resetting!" << std::endl;
            ps->currentTexture = "idle"; // Reset to idle texture after attack
        }
    }

    void onCreate() override {
        dirController.bind<changeDirectionController>(entity, *registry);
    }

    void onDraw() override {
        auto pt = getComponent<transform>();
        int x = static_cast<int>(pt->position.x);
        int y = static_cast<int>(pt->position.y);
        DrawText(("X: " + std::to_string(x) + " Y: " + std::to_string(y)).c_str(), 10, 30, 20, GREEN);

        auto pl = getComponent<lifeComponent>();
        DrawText(("Life: " + std::to_string(static_cast<int>(pl->life)) + "/" + std::to_string(static_cast<int>(pl->maxLife))).c_str(), 10, 50, 20, RED);
        auto ps = getComponent<staminaComponent>();
        DrawText(("Stamina: " + std::to_string(static_cast<int>(ps->stamina)) + "/" + std::to_string(static_cast<int>(ps->maxStamina))).c_str(), 10, 70, 20, BLUE);
    }

protected:
    script dirController;
};