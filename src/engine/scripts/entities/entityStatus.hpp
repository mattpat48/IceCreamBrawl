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

        if (statusComp->isAttacking() && pa->currentFrame == pa->endFrame) {
            std::cout << "Reset after attack" << std::endl;
            statusComp->status = IDLE;
            ps->currentTexture = "idle"; // Reset to idle texture after attack
            atk->currentCooldown = atk->cooldown; // Reset attack cooldown
            if (atk->modifier) {
                atk->cooldownModifier = (float)(rand()) / (float)(RAND_MAX);
                atk->currentCooldown += atk->cooldownModifier + 0.5f;
            }
            std::cout << "CD modifier: " << atk->cooldownModifier << std::endl;
            std::cout << "current CD: " << atk->currentCooldown << std::endl;
        }

        if (ps->currentTexture == "hurt" && pa->currentFrame == pa->endFrame) {
            std::cout << "Reset after hurt" << std::endl;
            statusComp->status = IDLE;
            ps->currentTexture = "idle"; // Reset to idle texture after attack
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