#pragma once

#include "components/components.hpp"
#include "scripts/general/controller.hpp"
#include "scripts/general/buttons.hpp"
#include "scripts/entities/entityStatus.hpp"
#include <entt/entt.hpp>

#include "raylib.h"
#include "raylib-cpp.hpp"

#include <iostream>

class playerScript : public Script {
public:

    playerScript(entt::entity player, entt::registry& registry) {
        this->entity = player; // Store the player entity reference
        this->registry = &registry; // Store the registry reference
    }

    void onUpdate(float dt) override {
        //dirController.instance->onUpdate(dt);
    }

    void onCreate() override {
        dirController.bind<changeDirectionController>(entity, *registry);
        playerStatus.bind<entityStatus>(entity, *registry);
    }

    void onDraw() override {
    }

    virtual ~playerScript() = default;

protected:
    entt::entity enemy;
    script dirController;
    script playerStatus;
};