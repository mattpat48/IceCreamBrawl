#pragma once

#include "components/components.hpp"
#include "scripts/script.hpp"
#include "scripts/general/controller.hpp"
#include "scripts/general/buttons.hpp"
#include "scripts/player/playerStatus.hpp"
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
        statusScript.instance->onUpdate(dt);
    }

    void onCreate() override {
        dirController.bind<changeDirectionController>(entity, *registry);
        statusScript.bind<playerStatus>(entity, *registry);
    }

    void onDraw() override {
    }

protected:
    script dirController;
    script statusScript;
};