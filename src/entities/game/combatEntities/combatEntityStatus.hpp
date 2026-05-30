#pragma once

#include "entt/entt.hpp"
#include <iostream>

#include "defines/general.hpp"
#include "defines/components/combatComponents.hpp"
#include "defines/components/entityComponents.hpp"
#include "entities/script.hpp"

class combatEntityStatus : public Script {
public:
    combatEntityStatus() {
        std::cout << "combatEntityStatus creato per entità: " << static_cast<uint32_t>(entity) << std::endl;
    }

    void onUpdate(float dt) override {
        auto healthComp = getComponent<health>();
        auto enduranceComp = getComponent<endurance>();
        auto statusComp = getComponent<combatStatus>();
        auto spriteComp = getComponent<sprite>();
        auto animationComp = getComponent<animation>();
        auto attackComp = getComponent<attack>();
        auto hitFlashComp = getComponent<hitFlash>();

        if (statusComp->status == CombatStatus::DEAD && animationComp->isPlaying) {
            if (animationComp->currentFrame == animationComp->endFrame) {
                animationComp->isPlaying = false;
            }
        }

        // Example logic to update player status
        if (healthComp->life <= 0 && statusComp->status != CombatStatus::DEAD) {
            statusComp->status = CombatStatus::DEAD; // Set status to DEAD
            std::cout << "Entity is dead!" << std::endl;
            spriteComp->currentTexture = "death"; // Change texture to death
            animationComp->currentFrame = animationComp->startFrame; // Reset animation frame to start frame
            return;
        }

        if (statusComp->status == CombatStatus::ATTACK && animationComp->currentFrame == animationComp->endFrame) {
            std::cout << "Reset after attack" << std::endl;
            statusComp->status = CombatStatus::IDLE;
            spriteComp->currentTexture = "idle"; // Reset to idle texture after attack
            attackComp->currentCooldown = attackComp->cooldown; // Reset attack cooldown
            if (attackComp->modifier) {
                attackComp->cooldownModifier = (float)(rand()) / (float)(RAND_MAX);
                attackComp->currentCooldown += attackComp->cooldownModifier + 0.5f;
            }
            std::cout << "CD modifier: " << attackComp->cooldownModifier << std::endl;
            std::cout << "current CD: " << attackComp->currentCooldown << std::endl;
        }

        if (hitFlashComp &&
            hitFlashComp->filter.r == RED.r &&
            hitFlashComp->filter.g == RED.g &&
            hitFlashComp->filter.b == RED.b &&
            hitFlashComp->filter.a == RED.a) {
            if (hitFlashComp->timeFlash >= 0.20f) {
                hitFlashComp->filter = WHITE;
                hitFlashComp->timeFlash = 0.0f;
            } else {
                std::cout << hitFlashComp->timeFlash << std::endl;
                hitFlashComp->timeFlash += dt;
            }
        }
        
        attackComp->updateCooldown(dt);
        
        //healthComp->regen(healthComp->regenRate * dt);
        enduranceComp->regen(enduranceComp->regenRate * dt + 5.0f);
    }

    void onCreate() override {
        // Initialization logic for player status
    }

    void onDraw() override {
    }
};