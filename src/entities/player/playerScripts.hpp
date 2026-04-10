#pragma once

#include "defines/components/components.hpp"
#include "ui/controller/controller.hpp"
#include "ui/buttons/buttons.hpp"
#include "entities/entityStatus.hpp"
#include "entities/entityAttack.hpp"
#include "entities/entityRegen.hpp"
#include "entities/entityMovement.hpp"
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
        playerStatus.instance->onDraw();
        playerAttack.instance->onDraw();
        playerRegen.instance->onDraw();
        playerMovement.instance->onDraw();
    }

    virtual ~playerScripts() = default;

protected:
    script playerStatus;
    script playerAttack;
    script playerRegen;
    script playerMovement;
};