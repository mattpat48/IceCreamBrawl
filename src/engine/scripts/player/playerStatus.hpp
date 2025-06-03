#pragma once

#include "components/components.hpp"
#include "scripts/script.hpp"
#include <entt/entt.hpp>
#include <iostream>

class playerStatus : public Script {
public:
    playerStatus(entt::entity entity, entt::registry& registry) {
        this->entity = entity; // Store the entity reference
        this->registry = &registry; // Store the registry reference
    }

    void onUpdate(float dt) override {
        auto healthComp = getComponent<health>();
        auto enduranceComp = getComponent<endurance>();
        auto statusComp = getComponent<status>();
        auto ps = getComponent<sprite>();
        auto pa = getComponent<animation>();
        auto atk = getComponent<attack>();

        // Example logic to update player status
        if (healthComp->life <= 0) {
            statusComp->status = DEAD;
            std::cout << "Player is dead!" << std::endl;
            return;
        }

        if (ps->currentTexture == "attack" && pa->currentFrame == pa->endFrame) {
            std::cout << "Player is resetting!" << std::endl;
            statusComp->status = IDLE;
            ps->currentTexture = "idle"; // Reset to idle texture after attack
            atk->currentCooldown = atk->cooldown; // Reset attack cooldown
        }
        
        atk->updateCooldown(dt);
    }

    void onCreate() override {
        // Initialization logic for player status
    }

    void onDraw() override {
        auto pt = getComponent<transform>();
        int x = static_cast<int>(pt->position.x);
        int y = static_cast<int>(pt->position.y);
        DrawText(("X: " + std::to_string(x) + " Y: " + std::to_string(y)).c_str(), 10, 30, 20, GREEN);

        auto pl = getComponent<health>();
        DrawText(("Life: " + std::to_string(static_cast<int>(pl->life)) + "/" + std::to_string(static_cast<int>(pl->maxLife))).c_str(), 10, 50, 20, RED);
        auto ps = getComponent<endurance>();
        DrawText(("Stamina: " + std::to_string(static_cast<int>(ps->stamina)) + "/" + std::to_string(static_cast<int>(ps->maxStamina))).c_str(), 10, 70, 20, BLUE);
    }
};