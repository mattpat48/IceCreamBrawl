#pragma once

#include "components/components.hpp"
#include "scripts/script.hpp"
#include <entt/entt.hpp>
#include <iostream>

class entityStatus : public Script {
public:
    entityStatus(entt::entity entity, entt::registry& registry) {
        this->entity = entity; // Store the entity reference
        this->registry = &registry; // Store the registry reference
        std::cout << "entityStatus creato per entità: " << static_cast<uint32_t>(entity) << std::endl;
    }

    void onUpdate(float dt) override {
        auto healthComp = getComponent<health>();
        auto enduranceComp = getComponent<endurance>();
        auto statusComp = getComponent<status>();
        auto ps = getComponent<sprite>();
        auto pa = getComponent<animation>();
        auto atk = getComponent<attack>();

        if (statusComp->isDead() && pa->isPlaying) {
            if (pa->currentFrame == pa->endFrame) {
                pa->isPlaying = false;
            }         
        }

        // Example logic to update player status
        if (healthComp->life <= 0 && !statusComp->isDead()) {
            statusComp->status = DEAD; // Set status to DEAD
            std::cout << "Entity is dead!" << std::endl;
            ps->currentTexture = "death"; // Change texture to death
            pa->currentFrame = pa->startFrame; // Reset animation frame to start frame
            return;
        }

        if (ps->currentTexture != "idle" &&
            ps->currentTexture != "death" &&
            pa->currentFrame == pa->endFrame) {
            statusComp->status = IDLE;
            ps->currentTexture = "idle"; // Reset to idle texture after attack
            atk->currentCooldown = atk->cooldown; // Reset attack cooldown
        }
        
        atk->updateCooldown(dt);
        
        enduranceComp->stamina += enduranceComp->regenRate * dt;
        if (enduranceComp->stamina > enduranceComp->maxStamina) {
            enduranceComp->stamina = enduranceComp->maxStamina; // Cap stamina at max
        }
    }

    void onCreate() override {
        // Initialization logic for player status
    }

    void onDraw() override {
    }
};