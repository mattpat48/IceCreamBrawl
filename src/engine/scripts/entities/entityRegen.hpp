#pragma once

#include "defines/components/components.hpp"
#include "engine/scripts/script.hpp"
#include <entt/entt.hpp>
#include <iostream>

class entityRegen : public Script {
public:

    void onCreate() override { }

    void onUpdate(float dt) override {
        auto healthComp = getComponent<health>();
        auto enduranceComp = getComponent<endurance>();
        
        healthComp->regen(healthComp->regenRate * dt);
		enduranceComp->regen(enduranceComp->regenRate * dt);
    }

    void onDraw() override { }
};