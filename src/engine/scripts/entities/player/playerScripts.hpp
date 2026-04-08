#pragma once

#include "defines/components/components.hpp"
#include "engine/scripts/ui/controller/controller.hpp"
#include "engine/scripts/ui/buttons/buttons.hpp"
#include "engine/scripts/entities/entityStatus.hpp"
#include "engine/scripts/entities/entityAttack.hpp"
#include "engine/scripts/entities/entityRegen.hpp"
#include "engine/scripts/entities/entityMovement.hpp"
#include <entt/entt.hpp>

#include "raylib.h"
#include "raylib-cpp.hpp"

#include <iostream>

class playerScripts : public Script {
public:

    void onCreate() override {
        playerStatus.bind<entityStatus>(entity, *registry);
        playerAttack.bind<entityAttack>(entity, *registry);
        playerRegen.bind<entityRegen>(entity, *registry);
        playerMovement.bind<entityMovement>(entity, *registry);
    }

    void onUpdate(float dt) override {
        playerStatus.instance->onUpdate(dt);
        playerAttack.instance->onUpdate(dt);
        playerRegen.instance->onUpdate(dt);
        playerMovement.instance->onUpdate(dt);
    }

    void onDraw() override {
    }

    virtual ~playerScripts() = default;

protected:
    script playerStatus;
    script playerAttack;
    script playerRegen;
    script playerMovement;
};