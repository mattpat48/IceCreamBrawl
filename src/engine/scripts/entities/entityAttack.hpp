#pragma once

#include "defines/components/components.hpp"
#include "engine/scripts/script.hpp"
#include <entt/entt.hpp>
#include <iostream>

class entityAttack : public Script {
public:

    void onUpdate(float dt) override {
        auto attackComp = getComponent<attack>();
        
        attackComp->updateCooldown(dt);
    }

    void onCreate() override { }

    void onDraw() override { }
};